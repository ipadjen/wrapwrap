//== GEOWRAPPER ==//

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
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>

namespace PMP = CGAL::Polygon_mesh_processing;

typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef K::FT                                                FT;

typedef K::Point_3                                           Point;
typedef std::array<FT, 3>                                    Custom_point;

typedef std::vector<size_t>                                  CGAL_Polygon;
typedef CGAL::Surface_mesh<Point>                            Mesh;

void printWelcome() {
    std::cout << "Hello to geowrapper!" << std::endl;
}

void printHelp() {
    auto helpMsg{
R"(USAGE:
    geowrapper inputfile relative_alpha[optional] relative_offset[optional]
)"
    };
    std::cout << helpMsg;
}

/*
 * Array traits for polygon soup repair
 */
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

/*
 * Polygon mesh processing for Alpha Wrap
 */
void prepGeom(std::vector<Point>& points, std::vector<CGAL_Polygon>& polygons, Mesh& mesh) {
    std::cout << "Preparing geometries for wrapping" << std::endl;

//    PMP::repair_polygon_soup(points, polygons, CGAL::parameters::geom_traits(Array_traits()));
    PMP::repair_polygon_soup(points, polygons);
    PMP::orient_polygon_soup(points, polygons);
    PMP::polygon_soup_to_polygon_mesh(points, polygons, mesh);
    PMP::triangulate_faces(mesh);
}

/*
 * Alpha wrap
 */
void wrap(Mesh& mesh, const double relative_alpha, const double relative_offset) {
    std::cout << "Wrapping geometries" << std::endl;

    CGAL::Bbox_3 bbox = CGAL::Polygon_mesh_processing::bbox(mesh);
    const double diag_length = std::sqrt(CGAL::square(bbox.xmax() - bbox.xmin()) +
                                         CGAL::square(bbox.ymax() - bbox.ymin()) +
                                         CGAL::square(bbox.zmax() - bbox.zmin()));
    const double alpha = diag_length / relative_alpha;
    const double offset = diag_length / relative_offset;
    CGAL::alpha_wrap_3(mesh, alpha, offset, mesh);

//    CGAL::alpha_wrap_3(newMesh, 2, 0.01, newMesh);   // 'coarse'
//    CGAL::alpha_wrap_3(mesh, 1.5, 0.03, mesh); // 'medium'
//    CGAL::alpha_wrap_3(newMesh, 0.7, 0.03, newMesh); // 'fine'

//    CGAL::alpha_wrap_3(newMesh, 0.3, 0.03, newMesh); // that one takes long time
//    CGAL::alpha_wrap_3(points, polygons, 0.1, 0.001, newMesh);
//    newMesh = wrap;
}


int main(int argc, char** argv) {
    std::cout.precision(17);
    try {
        auto startTime = std::chrono::steady_clock::now();
        printWelcome();
        if (argc < 2) {
            printHelp();
            return EXIT_SUCCESS;
        }

        //-- Input arguments
        const std::string filename = argv[1];
        const double relative_alpha = (argc > 2) ? std::stod(argv[2]) : 20.;
        const double relative_offset = (argc > 3) ? std::stod(argv[3]) : 600.;

        //-- Read the file as polygon soup
        std::cout << "Reading geometries" << std::endl;
        std::vector<Point> points;
        std::vector<CGAL_Polygon> polygons;
        if (!CGAL::IO::read_polygon_soup(filename, points, polygons)) {
            throw std::runtime_error("Cannot open file");
        };

        //-- Prepare polygon soup and turn it into polygon mesh
        Mesh mesh;
        prepGeom(points, polygons, mesh);

        //todo: hole filing before alpha wrap?

        //-- Conduct alpha wrap
        wrap(mesh, relative_alpha, relative_offset);

        //-- Output the data
        std::string input_name = std::string(filename);
        input_name = input_name.substr(input_name.find_last_of("/") + 1, input_name.length() - 1);
        input_name = input_name.substr(0, input_name.find_last_of("."));
        std::string output_name = input_name
                                  //                                  + "_" + std::to_string(static_cast<int>(relative_alpha))
                                  //                                  + "_" + std::to_string(static_cast<int>(relative_offset)) + ".obj";
                                  + "_wrapped.obj";
        std::cout << "Writing to " << output_name << std::endl;
        CGAL::IO::write_polygon_mesh(output_name, mesh, CGAL::parameters::stream_precision(17));

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
