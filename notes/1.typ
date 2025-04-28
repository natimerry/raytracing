= Ray, Camera, Background
We define a a ray as a function $P(t) = A + t B$ where P is any point and B is the ray direction. We can consider t to be a constant vector magnitude which changes per bounce (alongside B) and A is the origin of the ray.


= Circle formula

$x^2 + y^2 + z^2 =r^2$

To know if an arbitrary point is in the Circle:
$x^2 + y^2 + z^2 < r^2$.

I want a circle from point $P$ to an arbitrary point $C$ where $C$ is the centre of the circle.

$(C_x - x)^2 + (C_y - y)^2 + (C_z - z)^2 = r^2$

$C(C_x,C_y,C_z) - P "where" P "is" (x,y,z)$ and $ (C-P) dot (C-P) = r^2 $

$ (C-(A+t b)) dot (C-(A+t b)) = r^2  $

$ a = d dot d $
$ b = -2d dot (C-Q) $
$ c = (C-Q) dot (C - Q) $

