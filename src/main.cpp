
//https://www.raylib.com/cheatsheet/cheatsheet.html

//#include <windows.h>
#include "raylib.h"

#include "point.h"
#include "meshbuilder.h"
#include "math/vector.h"
#include "math/splines/spline.h"
#include "math/splines/catmullromspline.h"

std::vector<Vector3> GetOutline();
Mesh extrude(const std::vector<Vector3>& outline, const math::spline& s, double sampleRate);

struct car {
    double currentU{};
    double uSpeed{};
    double verticalOffset{};
    double offsetGoal{};
    Color color{};
};

int main(void) {
    //ShowWindow(GetConsoleWindow(), SW_HIDE);

    math::catmullrom_spline extrusionPath{
        { math::vec::vec3d(2, 4, 0), math::vec::vec3d(7, 0, 20), math::vec::vec3d(12, -4, 5),
            math::vec::vec3d(-12, 0, 17), math::vec::vec3d(-20, 2, 5)
            }
    };

    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "SplineCoaster");
    SetTargetFPS(60);

    Model model{ LoadModelFromMesh( extrude(GetOutline(), extrusionPath, 0.02f) ) };
    Camera camera = { { 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    std::vector<car> cars{
        { 0.1, 0.01, 0.3, 0.3, BLUE},
        { 0.2, 0.015, -0.2, -0.2, GREEN},
        { 0.3, 0.012, 0.1, 0.1, BEIGE},
        { 0.4, 0.013, 0.0, 0.0, BROWN},
        { 0.5, 0.0125, 0.76, 0.76, YELLOW},
        { 0.6, 0.018, -0.43, -0.43, MAROON},
        { 0.7, 0.02, -0.16, -0.16, VIOLET}
    };

    int currentlyLookedAtCarIndex = 4;
    int frameCount = 0;
    while (!WindowShouldClose()) {   // Detect window close button or ESC key
        
        //Change currently viewed car and camera offset
        double xOffset = 0;
        if(frameCount % (60*7) == 0) {
            currentlyLookedAtCarIndex = (currentlyLookedAtCarIndex + 1) % cars.size();
            xOffset = (GetRandomValue(0, 100) - 50) / 10.0;
        }

        //update cars
        for(car& c: cars) {
            c.currentU += c.uSpeed;
            if(c.currentU > extrusionPath.getSegmentCount()) c.currentU -= extrusionPath.getSegmentCount();

            c.verticalOffset += (c.offsetGoal - c.verticalOffset) / GetRandomValue(10, 25);
            if(std::abs(c.verticalOffset - c.offsetGoal) < 0.01) c.offsetGoal = (GetRandomValue(0, 40) - 20) / 20.0;
        }
        
        {   //update camera position
            const car& currentlyLookedAtCar{ cars.at(currentlyLookedAtCarIndex) };
            oriented_point carPosition{ extrusionPath.getOrientedPoint(currentlyLookedAtCar.currentU) };
            
            math::vec oldCameraPosition{ math::vec::vec3d(camera.position.x, camera.position.y, camera.position.z) };
            math::vec newCameraPositionHard{ carPosition.localToWorld(math::vec::vec3d(xOffset, -3, -6)) };
            newCameraPositionHard += math::vec::vec3d(0, std::abs(extrusionPath.getDerivate(currentlyLookedAtCar.currentU).get(1)), 0) * 5; //take heightchange of track in account

            math::vec newCameraPositionSoft{ 0.2 * newCameraPositionHard + 0.8 * oldCameraPosition }; //smoother blend between positions

            camera.position = newCameraPositionSoft.toVector3();
            camera.target = carPosition.position.toVector3();
        }
        UpdateCamera(&camera);

        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawRectangleGradientV(0, 0, screenWidth, screenHeight, SKYBLUE, RAYWHITE);

            BeginMode3D(camera);

              DrawModel(model, Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, PURPLE);
              DrawModelWires(model, Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, RED);

              for(car& c: cars) {
                oriented_point positionOnPath{ extrusionPath.getOrientedPoint(c.currentU) };

                math::vec carPos{ positionOnPath.position };
                carPos += math::vec::vec3d(0, 0.4, 0);
                carPos += positionOnPath.worldToLocalDirection(math::vec::vec3d(c.verticalOffset, 0, 0));

                DrawCube(carPos.toVector3(), 0.6f, 0.6f, 0.6f, c.color);
              }

            EndMode3D();

        EndDrawing();
        frameCount++;
    }

    CloseWindow();
    return 0;
}

std::vector<Vector3> GetOutline() {
    std::vector<Vector3> outline{};

    float scale = 0.25f;
    outline.push_back(Vector3{-4 * scale, -2 * scale});
    outline.push_back(Vector3{-3 * scale, -2 * scale});
    outline.push_back(Vector3{-2 * scale, -1 * scale});
    outline.push_back(Vector3{2 * scale, -1 * scale});
    outline.push_back(Vector3{3 * scale, -2 * scale});
    outline.push_back(Vector3{4 * scale, -2 * scale});

    return outline;
}

Mesh extrude(const std::vector<Vector3>& outline, const math::spline& s, double sampleRate) {
    std::vector<double> splinePath{};
    for(double i = 0; i <= s.getSegmentCount(); i += sampleRate) {
        splinePath.push_back(i);
    }

    /*
     *  +-----+-----+   Edgeloop 1, 3 vertices (+)
     *  i     i     i   \
     *  i     i     i    |
     *  i     i     i     > Segment 1
     *  i     i     i    |
     *  i     i     i   /
     *  +-----+-----+   Edgeloop 2
     * 
     * For each segment we have (vertices in outline/edgeloop -1) quads
     * Each quads needs two vertices
     */
    int vertsInShape = outline.size();
    int segments = splinePath.size() - 1;
    int edgeLoops = segments + 1;
    int triangles = (vertsInShape - 1) * segments * 2;

    std::vector<Vector3> vertices{};
    for(int i = 0; i < edgeLoops; i++) {
        double uValue = splinePath.at(i);

        oriented_point p{ s.getOrientedPoint(uValue) };
        for(int vi = 0; vi < vertsInShape; vi++) {
            Vector3 outlineinLocalCoordinates{ outline.at(vi) };
            Vector3 outlineInWorldCoordinates{ p.localToWorld( math::vec::vec3d(outlineinLocalCoordinates.x, outlineinLocalCoordinates.y, outlineinLocalCoordinates.z) ).toVector3() };
            vertices.push_back(outlineInWorldCoordinates);
        }
    }

    //connect vertices with triangles
    meshbuilder builder{ triangles };
    for(int loop = 0; loop < edgeLoops - 1; loop++) {
        int startIndexFirstLoop = vertsInShape * loop;
        int startIndexSecondLoop = vertsInShape + startIndexFirstLoop;

        for(int vi = 0; vi < vertsInShape - 1; vi++) {
            Vector3 v1_loop1 = vertices.at(startIndexFirstLoop + vi);
            Vector3 v2_loop1 = vertices.at(startIndexFirstLoop + vi + 1);
            Vector3 v1_loop2 = vertices.at(startIndexSecondLoop + vi);
            Vector3 v2_loop2 = vertices.at(startIndexSecondLoop + vi + 1);

            builder.addTriangle(v1_loop1, v2_loop1, v1_loop2);
            builder.addTriangle(v1_loop2, v2_loop1, v2_loop2);
        }
    }

    return builder.build();
}