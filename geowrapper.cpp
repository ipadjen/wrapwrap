#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/alpha_wrap_3.h>

#include <CGAL/Polygon_mesh_processing/bbox.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>

namespace PMP = CGAL::Polygon_mesh_processing;

typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef K::FT                                                FT;

typedef K::Point_3                                           Point;
typedef std::array<FT, 3>                                    Custom_point;

typedef std::vector<size_t>                                  CGAL_Polygon;
typedef CGAL::Surface_mesh<Point>                            Mesh;
typedef boost::graph_traits<Mesh>::halfedge_descriptor       halfedge_descriptor;

void printWelcome() {
    std::cout << "geowrapper" << std::endl << std::endl;
}

void printHelp() {
    auto helpMsg{
R"(USAGE:
    geowrapper -i input_file.obj -alpha realative_alpha -offset relative_offset -o output_file.obj
)"
    };
    std::cout << helpMsg;
}

/*
 * Array traits for polygon soup repair
 */
/*
struct Array_traits
{
    struct Equal_3
    {
        bool operator()(const Custom_point& p, const Custom_point& q) const {
            return (p == q);
        }
    };
    struct Less_xyz_3
    {
        bool operator()(const Custom_point& p, const Custom_point& q) const {
            return std::lexicographical_compare(p.begin(), p.end(), q.begin(), q.end());
        }
    };
    Equal_3 equal_3_object() const { return Equal_3(); }
    Less_xyz_3 less_xyz_3_object() const { return Less_xyz_3(); }
};
*/

/*
 * Polygon mesh processing for Alpha Wrap
 */
void prepGeom(std::vector<Point>& points, std::vector<CGAL_Polygon>& polygons, Mesh& mesh) {
    std::cout << "Preparing geometries for wrapping..." << std::endl;
//    PMP::repair_polygon_soup(points, polygons, CGAL::parameters::geom_traits(Array_traits()));
    PMP::repair_polygon_soup(points, polygons);
    PMP::orient_polygon_soup(points, polygons);
    PMP::polygon_soup_to_polygon_mesh(points, polygons, mesh);
    PMP::triangulate_faces(mesh);

    std::cout << "  done" << std::endl;
}

/*
 * Fill open boundaries
 */
void fillHoles(Mesh& mesh) {
  if (!CGAL::is_closed(mesh)) {
    std::cout << "Mesh open, filling holes..." << std::endl;
    // collect boundary halfedges
    std::vector<halfedge_descriptor> border_cycles;
    PMP::extract_boundary_cycles(mesh, std::back_inserter(border_cycles));

    // fill using boundary halfedges
    for(halfedge_descriptor h : border_cycles)
      PMP::triangulate_hole(mesh, h);

    std::cout << "  done" << std::endl;
  }
}

/*
 * Alpha wrap
 */
void wrap(Mesh& mesh, const double relative_alpha, const double relative_offset) {
    std::cout << "Wrapping geometries..." << std::endl;

    CGAL::Bbox_3 bbox = PMP::bbox(mesh);
    const double diag_length = std::sqrt(CGAL::square(bbox.xmax() - bbox.xmin()) +
                                         CGAL::square(bbox.ymax() - bbox.ymin()) +
                                         CGAL::square(bbox.zmax() - bbox.zmin()));
    const double alpha = diag_length / relative_alpha;
    const double offset = diag_length / relative_offset;
    CGAL::alpha_wrap_3(mesh, alpha, offset, mesh);

    std::cout << "  done" << std::endl;
}


int main(int argc, char** argv) {
    std::cout.precision(6);
    try {
        auto startTime = std::chrono::steady_clock::now();
        printWelcome();
        if (argc < 2) {
            printHelp();
            return EXIT_SUCCESS;
        }

        //-- Input arguments
        std::string inputFile, outputFile;
        std::optional<double> relativeAlpha, relativeOffset;
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];

            if (arg == "-i" && i + 1 < argc) {
                inputFile = argv[i + 1];
                i++;
            } else if (arg == "-alpha" && i + 1 < argc) {
                relativeAlpha = std::stod(argv[i + 1]);
                i++;
            } else if (arg == "-offset" && i + 1 < argc) {
                relativeOffset = std::stod(argv[i + 1]);
                i++;
            } else if (arg == "-o" && i + 1 < argc) {
                outputFile = argv[i + 1];
                i++;
            }
        }
        // Check if all required arguments are provided
        if (inputFile.empty() || outputFile.empty() || !relativeAlpha || !relativeOffset) {
            std::cout << "Missing or invalid arguments. Usage: geowrapper -i input_file.obj "
                         "-alpha relative_alpha -offset relative_offset -o output_file" << std::endl;
            return 1;
        }

        //-- Read the file as a polygon soup
        std::cout << "Reading geometries..." << std::endl;
        std::vector<Point> points;
        std::vector<CGAL_Polygon> polygons;
        if (!CGAL::IO::read_polygon_soup(inputFile, points, polygons)) {
            throw std::runtime_error("Cannot open file");
        };

        //-- Prepare polygon soup and turn it into polygon mesh
        Mesh mesh;
        prepGeom(points, polygons, mesh);

        //-- Hole filling
        fillHoles(mesh);

        //-- Conduct alpha wrap
        wrap(mesh, relativeAlpha.value(), relativeOffset.value());

        //-- Output the data
        std::cout << "Writing to " << outputFile << "..." << std::endl;
        CGAL::IO::write_polygon_mesh(outputFile, mesh, CGAL::parameters::stream_precision(17));
        std::cout << "  done" << std::endl;

        //-- Measure time and end
        auto endTime = std::chrono::steady_clock::now();
        auto diffTime = endTime - startTime;
        std::cout << "\nProgram executed in " << std::chrono::duration<double>(diffTime).count() << " s" << std::endl;
        std:: cout << "End" << std::endl;

        return EXIT_SUCCESS;
    } catch (std::exception& e) {
        //-- Output log

        std::cerr << "\nProgram failed! Reason: " << e.what() << std::endl;
        std::cout << "End" << std::endl;
        return EXIT_FAILURE;
    }
}
