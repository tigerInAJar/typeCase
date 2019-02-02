#include "planarShapes.h"
#include "provideShapes.h"
#include "shapeparameter.h"

namespace {

const boost::uuids::uuid hexagon_id = {{0x49, 0xde, 0xdb, 0xfa, 0x11, 0xeb, 0x11, 0xe9, 0xab, 0x14,
                                        0xd6, 0x63, 0xbd, 0x87, 0x3d, 0x93}};

struct ShapeProvider : public FactoryShapeProvider,
                       public std::enable_shared_from_this<ShapeProvider>
{
  void addToFactory(ShapeFactory& factory) const override
  {
    factory.addShapeToFactory(
        hexagon::getDescription(), ShapeType::PlanarShape,
        [](shape_parameter const& param) -> std::shared_ptr<planeShape> {
          return std::shared_ptr<planeShape>(new hexagon(
              param.getParam<point3D>(0), param.getParam<point3D>(1), param.getParam<vector3D>(0)));
        },
        [](shape_parameter const&, size_t) -> shape_parameter { return {}; },
        [](shape_parameter const&, size_t) -> shape_parameter { return {}; });
  }
  void removeFromFactory(ShapeFactory& factory) const override
  {
    factory.removeShapeFromFactory(hexagon_id);
  }
  void install() { Shape::innerShapeProviders.push_back(shared_from_this()); }
};
std::shared_ptr<ShapeProvider> prov = [] {
  auto r = std::make_shared<ShapeProvider>();
  r->install();
  return r;
}();
}

hexagon::hexagon(const planeShape& p) : planeShape("hexagon")
{
  center = p.getCenter();
  normal = p.getNormal();
  circRadius = -1;
}
hexagon::hexagon(point3D cen, point3D a, vector3D norm) : planeShape("hexagon")
{
  center = cen;
  normal = norm;
  normal.normalize();
  A_ = a;
  circRadius = (a - cen).R();
}
hexagon::hexagon(const shape_parameter& description) : planeShape("hexagon")
{
  if (description.getName() != "hexagon")
    return;
  if (description.NumberOfParams<point3D>() < 2 || description.NumberOfParams<vector3D>() < 1)
    return;
  center = description.getParam<point3D>(0);
  normal = description.getParam<vector3D>(0);
  normal.normalize();
  A_ = description.getParam<point3D>(1);
  circRadius = (A_ - center).R();
}
int hexagon::getNumberOfPoints() const { return 6; }
void hexagon::setPoints(point3D cen, point3D a, vector3D norm)
{
  center = cen;
  normal = norm;
  normal.normalize();
  A_ = a;
  circRadius = (a - cen).R();
}

point3D hexagon::B() const
{
  vector3D tmp = A_ - center;
  matrix3D turn(normal, M_PI / 3);
  return center + (turn * tmp);
}

point3D hexagon::C() const
{
  vector3D tmp = A_ - center;
  matrix3D turn(normal, 2 * M_PI / 3);
  return center + (turn * tmp);
}

point3D hexagon::D() const { return (center - A_) + center; }

point3D hexagon::E() const
{
  vector3D tmp = A_ - center;
  matrix3D turn(normal, 4 * M_PI / 3);
  return center + (turn * tmp);
}

point3D hexagon::F() const
{
  vector3D tmp = A_ - center;
  matrix3D turn(normal, 5 * M_PI / 3);
  return center + (turn * tmp);
}

float hexagon::area() const
{
  triangle tmp(center, A_, B());
  return 6 * tmp.area();
}

lLine3D hexagon::AB() const { return lLine3D(A_, B()); }

lLine3D hexagon::BC() const { return lLine3D(B(), C()); }

lLine3D hexagon::CD() const { return lLine3D(C(), D()); }

lLine3D hexagon::DE() const { return lLine3D(D(), E()); }

lLine3D hexagon::EF() const { return lLine3D(E(), F()); }

lLine3D hexagon::FA() const { return lLine3D(F(), A_); }

void hexagon::operator=(const hexagon& h)
{
  center = h.getCenter();
  normal = h.getNormal();
  A_ = h.A();
}
hexagon::~hexagon() {}

point3D hexagon::getPoint(int num) const
{
  vector3D tmp = A_ - center;
  matrix3D turn(normal, M_PI / 3);
  switch (num) {
  case 5:
    tmp = turn * tmp;
  case 4:
    tmp = turn * tmp;
  case 3:
    tmp = turn * tmp;
  case 2:
    tmp = turn * tmp;
  case 1:
    tmp = turn * tmp;
  case 0:
    return center + tmp;
  }
  return point3D();
}

void hexagon::setA(point3D p) { A_ = p; }

point3D hexagon::A() const { return A_; }

float hexagon::angularRange_Phi(const point3D& origin, const vector3D& zDirection) const
{
  vector3D v[6];
  v[0] = A() - origin, v[1] = B() - origin, v[2] = C() - origin;
  v[3] = D() - origin, v[4] = E() - origin, v[5] = F() - origin;
  vector3D vc = center - origin;
  vector3D n[6], nc;
  nc = vc - zDirection * (vc * zDirection);
  nc.normalize();
  float t[6];
  for (int i = 0; i < 6; i++) {
    v[i].normalize();
    n[i] = v[i] - zDirection * (v[i] * zDirection);
    n[i].normalize();
    t[i] = n[i] * nc;
  }
  float min = t[0], max = t[0];
  for (int i = 1; i < 6; i++) {
    if (min > t[i])
      min = t[i];
    if (max < t[i])
      max = t[i];
  }
  return max - min;
}
float hexagon::angularRange_Theta(const point3D& origin, const vector3D& zDirection) const
{
  vector3D v[6];
  v[0] = A() - origin, v[1] = B() - origin, v[2] = C() - origin;
  v[3] = D() - origin, v[4] = E() - origin, v[5] = F() - origin;
  vector3D vc = center - origin;
  float t[6];
  for (int i = 0; i < 6; i++) {
    v[i].normalize();
    t[i] = acos(v[i] * zDirection);
  }
  float min = t[0], max = t[0];
  for (int i = 1; i < 6; i++) {
    if (min > t[i])
      min = t[i];
    if (max < t[i])
      max = t[i];
  }
  return max - min;
}
vector3D hexagon::distancePlane(const plane3D& p)
{
  if ((p.Normal() * normal) - 1 <= __prec)
    return p & center;
  vector3D v[6];
  v[0] = p & A();
  v[1] = p & B();
  v[2] = p & C();
  v[3] = p & D();
  v[4] = p & E();
  v[5] = p & F();
  for (int i = 0; i < 6; i++)
    for (int j = i + 1; j < 6; j++)
      if (v[i] * v[j] < 0)
        return vector3D(0, 0, 0);
  int m = 0;
  for (int i = 1; i < 6; i++)
    if (v[i].length() < v[m].length())
      m = i;
  return v[m];
}
vector3D hexagon::distance(const sLine3D& line)
{
  point3D hit(getPlane() - line);
  vector4D l = center - hit;
  l = l * (l.R() - (center - A_).R());
  if (l * (A_ - center) < 0)
    return vector3D(0, 0, 0);
  return l;
}

extern void DrawPoints(int nPoints, point3D* points, const point3D& eye, const plane3D& plane,
                       vector4D* boundingBox, TObject** ident, int lColor, int fColor, int fStyle);
void hexagon::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, TObject** ident,
                   int lColor, int fColor, int fStyle) const
{
  point3D ps[6] = {getPoint(0), getPoint(1), getPoint(2), getPoint(3), getPoint(4), getPoint(5)};
  DrawPoints(6, ps, eye, plane, boundingBox, ident, lColor, fColor, fStyle);
}
void hexagon::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                   int fColor, int fStyle) const
{
  Draw(eye, plane, boundingBox, NULL, lColor, fColor, fStyle);
}
shape_parameter hexagon::description() const
{
  shape_parameter sh;
  sh.setName("hexagon");
  sh.setId(hexagon_id);
  sh.addParam<point3D>(center, "center");
  sh.addParam<point3D>(A_, "A");
  sh.addParam<vector3D>(normal, "normal");
  sh.setCompleteWrite(true);
  return sh;
}
shape_parameter hexagon::getDescription()
{
  shape_parameter sh;
  sh.setName("hexagon");
  sh.setId(hexagon_id);
  sh.addParam<point3D>(point3D(), "center");
  sh.addParam<point3D>(point3D(), "A");
  sh.addParam<vector3D>(vector3D(), "normal");
  sh.setCompleteWrite(false);
  return sh;
}
