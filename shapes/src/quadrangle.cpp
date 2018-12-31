#include "quadrangle.h"
#include "shapeparameter.h"
quadrangle::quadrangle(point3D cent, point3D a, point3D b, point3D c, point3D d)
    : planeShape("quadrangle")
{
  center = cent;
  A_ = a;
  B_ = b;
  normal = (a - cent) ^ (b - cent);
  normal.normalize();
  C_ = cent + ((c - cent) - (normal * ((c - cent) * normal)));
  D_ = cent + ((d - cent) - (normal * ((d - cent) * normal)));
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center), v4(D_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
  if (v4.R() > circRadius)
    circRadius = v4.R();
}
quadrangle::quadrangle(const shape_parameter& description) : planeShape("quadrangle")
{
  if (description.getName() != "quadrangle")
    return;
  if (description.NumberOfParams<point3D>() < 5)
    return;
  center = description.getParam<point3D>(0);
  A_ = description.getParam<point3D>(1);
  B_ = description.getParam<point3D>(2);
  normal = (A_ - center) ^ (B_ - center);
  normal.normalize();
  C_ = center + ((description.getParam<point3D>(3) - center) -
                 (normal * ((description.getParam<point3D>(3) - center) * normal)));
  D_ = center + ((description.getParam<point3D>(4) - center) -
                 (normal * ((description.getParam<point3D>(4) - center) * normal)));
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center), v4(D_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
  if (v4.R() > circRadius)
    circRadius = v4.R();
}
quadrangle::~quadrangle() {}
int quadrangle::getNumberOfPoints() const { return 4; }
point3D quadrangle::getPoint(int num) const
{
  switch (num) {
  case 0:
    return A_;
    break;
  case 1:
    return B_;
    break;
  case 2:
    return C_;
    break;
  case 3:
    return D_;
    break;
  }
  return point3D();
}
float quadrangle::area() const
{
  triangle t1(A_, B_, C_), t2(B_, C_, D_);
  return t1.area() + t2.area();
}
float quadrangle::angularRange_Phi(const point3D& origin, const vector3D& zDirection) const
{
  vector3D v[4];
  v[0] = A() - origin, v[1] = B() - origin, v[2] = C() - origin;
  v[3] = D() - origin;
  vector3D vc = center - origin;
  vector3D n[4], nc;
  nc = vc - zDirection * (vc * zDirection);
  nc.normalize();
  float t[4];
  for (int i = 0; i < 4; i++) {
    v[i].normalize();
    n[i] = v[i] - zDirection * (v[i] * zDirection);
    n[i].normalize();
    t[i] = n[i] * nc;
  }
  float min = t[0], max = t[0];
  for (int i = 1; i < 4; i++) {
    if (min > t[i])
      min = t[i];
    if (max < t[i])
      max = t[i];
  }
  return max - min;
}
float quadrangle::angularRange_Theta(const point3D& origin, const vector3D& zDirection) const
{
  vector3D v[4];
  v[0] = A() - origin, v[1] = B() - origin, v[2] = C() - origin;
  v[3] = D() - origin;
  vector3D vc = center - origin;
  float t[4];
  for (int i = 0; i < 4; i++) {
    v[i].normalize();
    t[i] = acos(v[i] * zDirection);
  }
  float min = t[0], max = t[0];
  for (int i = 1; i < 4; i++) {
    if (min > t[i])
      min = t[i];
    if (max < t[i])
      max = t[i];
  }
  return max - min;
}
point3D quadrangle::A() const { return A_; }
point3D quadrangle::B() const { return B_; }

point3D quadrangle::C() const { return C_; }

point3D quadrangle::D() const { return D_; }

void quadrangle::setA(point3D p)
{
  A_ = center + ((p - center) - (normal * ((p - center) * normal)));
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center), v4(D_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
  if (v4.R() > circRadius)
    circRadius = v4.R();
}
void quadrangle::setB(point3D p)
{
  B_ = center + ((p - center) - (normal * ((p - center) * normal)));
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center), v4(D_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
  if (v4.R() > circRadius)
    circRadius = v4.R();
}
void quadrangle::setC(point3D p)
{
  C_ = center + ((p - center) - (normal * ((p - center) * normal)));
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center), v4(D_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
  if (v4.R() > circRadius)
    circRadius = v4.R();
}
void quadrangle::setD(point3D p)
{
  D_ = center + ((p - center) - (normal * ((p - center) * normal)));
  vector3D v1(A_ - center), v2(B_ - center), v3(C_ - center), v4(D_ - center);
  circRadius = v1.R();
  if (v2.R() > circRadius)
    circRadius = v2.R();
  if (v3.R() > circRadius)
    circRadius = v3.R();
  if (v4.R() > circRadius)
    circRadius = v4.R();
}

vector3D quadrangle::distancePlane(const plane3D& p)
{
  if ((p.Normal() * normal) - 1 <= __prec)
    return p & center;
  vector3D v[4];
  v[0] = p & A();
  v[1] = p & B();
  v[2] = p & C();
  v[3] = p & D();
  for (int i = 0; i < 4; i++)
    for (int j = i + 1; j < 4; j++)
      if (v[i] * v[j] < 0)
        return vector3D(0, 0, 0);
  int m = 0;
  for (int i = 1; i < 4; i++)
    if (v[i].length() < v[m].length())
      m = i;
  return v[m];
}
vector3D quadrangle::distance(const sLine3D& line)
{
  point3D hit(getPlane() - line);
  vector3D aa = A_ - hit, bb = B_ - hit, cc = C_ - hit, dd = D_ - hit;
  float a = acos((aa * bb) / aa.R() / bb.R()), b = acos((bb * cc) / bb.R() / cc.R()),
        c = acos((cc * dd) / cc.R() / dd.R()), d = acos((aa * dd) / aa.R() / dd.R());
  if ((a + b + c + d - M_PI * 2.) > -0.01)
    return vector3D(0, 0, 0);
  if ((a + b + c + d - M_PI) < 0) {
    if (aa.R() < bb.R() && aa.R() < cc.R() && aa.R() < dd.R())
      return aa;
    if (bb.R() < cc.R() && bb.R() < dd.R())
      return bb;
    if (cc.R() < dd.R())
      return cc;
    return dd;
  }
  if (a > b && a > c && a > d)
    return (bb + aa) * 0.5;
  if (b > c && b > d)
    return (bb + cc) * 0.5;
  if (c > d)
    return (cc + dd) * 0.5;
  return (dd + aa) * 0.5;
}
extern void DrawPoints(int nPoints, point3D* points, const point3D& eye, const plane3D& plane,
                       vector4D* boundingBox, TObject** ident, int lColor, int fColor, int fStyle);
void quadrangle::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox,
                      TObject** ident, int lColor, int fColor, int fStyle) const
{
  point3D ps[4] = {getPoint(0), getPoint(1), getPoint(2), getPoint(3)};
  DrawPoints(4, ps, eye, plane, boundingBox, ident, lColor, fColor, fStyle);
}
void quadrangle::Draw(const point3D& eye, const plane3D& plane, vector4D* boundingBox, int lColor,
                      int fColor, int fStyle) const
{
  Draw(eye, plane, boundingBox, NULL, lColor, fColor, fStyle);
}
shape_parameter quadrangle::description() const
{
  shape_parameter sh;
  sh.setName("quadrangle");
  sh.setCompleteWrite(true);
  sh.addParam<point3D>(center, "center");
  sh.addParam<point3D>(A_, "A");
  sh.addParam<point3D>(B_, "B");
  sh.addParam<point3D>(C_, "C");
  sh.addParam<point3D>(D_, "D");
  return sh;
}
shape_parameter quadrangle::getDescription()
{
  shape_parameter sh;
  sh.setName("quadrangle");
  sh.addParam<point3D>(point3D(), "center");
  sh.addParam<point3D>(point3D(), "A");
  sh.addParam<point3D>(point3D(), "B");
  sh.addParam<point3D>(point3D(), "C");
  sh.addParam<point3D>(point3D(), "D");
  sh.setCompleteWrite(false);
  return sh;
}
