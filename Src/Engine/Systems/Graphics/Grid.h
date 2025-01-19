#ifndef GLSLCOOKBOOK_GRID_H
#define GLSLCOOKBOOK_GRID_H

#include <GL/glew.h>

#include <vector>

class Grid
{
private:
    std::vector<GLuint> buffers;
    GLuint vao;
    GLsizei nVerts;

public:
    Grid( float size = 10.0f, int nDivisions = 10 );
    ~Grid();
    void render() const;
};

#endif //GLSLCOOKBOOK_GRID_H
