#pragma once

#include <axmol.h>

#include <vector>
#include <string>

using Axis = ax::Vec2;

class Collision
{
private:

    class Projection
    {
    public:
        Projection(float min, float max) : min(min), max(max) {};

        float length() {
            return max - min;
        }

        bool overlap(Projection other)
        {
            if (max < other.min || min > other.max)
                return false;
            return true;
        }
        float getOverlap(Projection other) {
            return std::max(std::min(max, other.max) - std::max(min, other.min), 0.f);
        }

        bool contains(Projection other) {
            //This containe other
            //If overlap equal length of other
            return getOverlap(other) == other.length();
        }

        float min, max;
    };


    class Polygon
    {
    public:
        Polygon(std::vector<ax::Vec2> vertices) {
            this->vertices.clear();
            for (auto vertex : vertices)
            {
                this->vertices.push_back(vertex);
            }
        }
        std::vector<ax::Vec2> vertices;

        std::vector<Axis> getAxes() {

            std::vector<Axis> axes;
            for (int i = 0; i < vertices.size(); i++)
            {
                ax::Vec2 point1                       = vertices.at(i);
                ax::Vec2 point2                       = vertices.at((i + 1) % vertices.size());
                ax::Vec2 edge                         = point2 - point1;
                ax::Vec2 perpendicularVector          = edge.getPerp();
                ax::Vec2 normalizePerpendicularVector = perpendicularVector.getNormalized();
                axes.push_back(Axis(normalizePerpendicularVector));
            }
            return axes;
        }

        Projection project(Axis axis) {
            float min = axis.dot(vertices[0]);
            float max = min;
            for (auto vertex : vertices)
            {
                float projected = axis.dot(vertex);
                if (projected < min)
                    min = projected;
                else if (projected > max)
                    max = projected;
            }
            return Projection(min, max);
        }
    };

public:
    

    static bool overlap(Polygon polygon1, Polygon polygon2, ax::Vec2& minimumTranslationVector);

    // Main game will use these
    static bool overlap(ax::Node* node1, ax::Node* node2, ax::Vec2& minimumTranslationVector);  // mtv: minimumTranslationVector
    static bool overlap(ax::Node* node, ax::Vec2 worldPoint);


};
