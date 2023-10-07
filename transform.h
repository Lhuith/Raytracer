// transform header file to define the interface.
// The class is all static for simplicity
// You need to implement left, up and lookAt
// Rotate is a helper function

// Include the helper glm library, including matrix transform extensions

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// glm provides vector, matrix classes like glsl
// Typedefs to make code more readable

typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
const float pi = 3.14159265; // For portability across platforms

class transform
{
public:
    transform();
    virtual ~transform();
    static void left(float degrees, vec3 &eye, vec3 &up);
    static void up(float degrees, vec3 &eye, vec3 &up);
    static mat4 lookAt(const vec3 &eye, const vec3 &center, const vec3 &up);
    static mat4 perspective(float fovy, float aspect, float zNear, float zFar);
    static mat3 rotate(const float degrees, const vec3 &axis);
    static mat4 scale(const float &sx, const float &sy, const float &sz);
    static mat4 translate(const float &tx, const float &ty, const float &tz);
    static vec3 upvector(const vec3 &up, const vec3 &zvec);
};

// Helper rotation function.  Please implement this.
mat3 transform::rotate(const float degrees, const vec3 &axis)
{
    // axis / a
    // float radians = rad degrees * pi / 180;
    float radians = glm::radians(degrees);

    mat3 i;

    i[0][0] = 1;
    i[1][0] = 0;
    i[2][0] = 0;

    i[0][1] = 0;
    i[1][1] = 1;
    i[2][1] = 0;

    i[0][2] = 0;
    i[1][2] = 0;
    i[2][2] = 1;

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    mat3 a_at;
    a_at[0][0] = x * x;
    a_at[1][0] = y * x;
    a_at[2][0] = z * x;

    a_at[0][1] = x * y;
    a_at[1][1] = y * y;
    a_at[2][1] = z * y;

    a_at[0][2] = x * z;
    a_at[1][2] = y * z;
    a_at[2][2] = z * z;

    // [c][r]
    mat3 perp;
    perp[0][0] = 0;
    perp[1][0] = -z;
    perp[2][0] = y;

    perp[0][1] = z;
    perp[1][1] = 0;
    perp[2][1] = -x;

    perp[0][2] = -y;
    perp[1][2] = x;
    perp[2][2] = 0;

    mat3 rot = cos(radians) * i + (1 - cos(radians)) * a_at + sin(radians) * perp;
    return rot;
}

void transform::left(float degrees, vec3 &eye, vec3 &up)
{
    eye = rotate(degrees, up) * eye;
}

void transform::up(float degrees, vec3 &eye, vec3 &up)
{
    vec3 axis = normalize(cross(eye, up));
    eye = rotate(degrees, axis) * eye;
    up = rotate(degrees, axis) * up;
}

mat4 transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
    mat4 rot;

    vec3 c = vec3(0.0, 0.0, 0.0);
    vec3 b = up;

    vec3 w = glm::normalize(eye);
    vec3 u = normalize(cross(b, w));
    vec3 v = cross(w, u);

    // [c][r]
    rot[0][0] = u.x;
    rot[1][0] = u.y;
    rot[2][0] = u.z;
    rot[3][0] = -dot(u, eye);
    rot[0][1] = v.x;
    rot[1][1] = v.y;
    rot[2][1] = v.z;
    rot[3][1] = -dot(v, eye);
    rot[0][2] = w.x;
    rot[1][2] = w.y;
    rot[2][2] = w.z;
    rot[3][2] = -dot(w, eye);
    rot[0][3] = 0;
    rot[1][3] = 0;
    rot[2][3] = 0;
    rot[3][3] = 1;

    return rot;
}

mat4 transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 rot;

    float angle = (fovy / 2);

    // cot(x) = cos(x)/sin(x)
    float d = cos(angle) / sin(angle);
    float A = (zFar + zNear) / (zFar - zNear);
    float B = (2 * (zFar * zNear)) / (zFar - zNear);
    rot[0][0] = d / aspect;
    rot[1][0] = 0;
    rot[2][0] = 0;
    rot[3][0] = 0;
    rot[0][1] = 0;
    rot[1][1] = d;
    rot[2][1] = 0;
    rot[3][1] = 0;
    rot[0][2] = 0;
    rot[1][2] = 0;
    rot[2][2] = -A;
    rot[3][2] = -B;
    rot[0][3] = 0;
    rot[1][3] = 0;
    rot[2][3] = -1;
    rot[3][3] = 0;
    return rot;
}

mat4 transform::scale(const float &sx, const float &sy, const float &sz)
{
    mat4 rot;
    rot[0][0] = sx;
    rot[1][0] = 0;
    rot[2][0] = 0;
    rot[3][0] = 0;
    rot[0][1] = 0;
    rot[1][1] = sy;
    rot[2][1] = 0;
    rot[3][1] = 0;
    rot[0][2] = 0;
    rot[1][2] = 0;
    rot[2][2] = sz;
    rot[3][2] = 0;
    rot[0][3] = 0;
    rot[1][3] = 0;
    rot[2][3] = 0;
    rot[3][3] = 1;
    return rot;
}

mat4 transform::translate(const float &tx, const float &ty, const float &tz)
{
    mat4 rot;
    rot[0][0] = 1;
    rot[1][0] = 0;
    rot[2][0] = 0;
    rot[3][0] = tx;
    rot[0][1] = 0;
    rot[1][1] = 1;
    rot[2][1] = 0;
    rot[3][1] = ty;
    rot[0][2] = 0;
    rot[1][2] = 0;
    rot[2][2] = 1;
    rot[3][2] = tz;
    rot[0][3] = 0;
    rot[1][3] = 0;
    rot[2][3] = 0;
    rot[3][3] = 1;
    return rot;
}

vec3 transform::upvector(const vec3 &up, const vec3 &zvec)
{
    vec3 x = glm::cross(up, zvec);
    vec3 y = glm::cross(zvec, x);
    vec3 ret = glm::normalize(y);
    return ret;
}

transform::transform()
{
}

transform::~transform()
{
}