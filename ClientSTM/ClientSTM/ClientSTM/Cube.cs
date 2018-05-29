using System;

namespace Forms3D
{
    public class Cube : PolyhedronBase
    {
        static readonly Point3D[,] faces = new Point3D[6, 5]
        {
            // front
            { new Point3D(0, 0, 0.25), 
              new Point3D(-0.25, 0.25, 0.25), new Point3D(-0.25, -0.25, 0.25), 
              new Point3D(0.25, -0.25, 0.25), new Point3D(0.25, 0.25, 0.25) },
            // back
            { new Point3D(0, 0, -0.25),
              new Point3D(0.25, 0.25, -0.25), new Point3D(0.25, -0.25, -0.25),
              new Point3D(-0.25, -0.25, -0.25), new Point3D(-0.25, 0.25, -0.25) },
            // left
            { new Point3D(-0.25, 0, 0),
              new Point3D(-0.25, 0.25, -0.25), new Point3D(-0.25, -0.25, -0.25),
              new Point3D(-0.25, -0.25, 0.25), new Point3D(-0.25, 0.25, 0.25) },
            // right
            { new Point3D(0.25, 0, 0),
              new Point3D(0.25, 0.25, 0.25), new Point3D(0.25, -0.25, 0.25),
              new Point3D(0.25, -0.25, -0.25), new Point3D(0.25, 0.25, -0.25) },
            // top
            { new Point3D(0, 0.25, 0),
              new Point3D(-0.25, 0.25, -0.25), new Point3D(-0.25, 0.25, 0.25),
              new Point3D(0.25, 0.25, 0.25), new Point3D(0.25, 0.25, -0.25) },
            // bottom
            { new Point3D(0, -0.25, 0),
              new Point3D(-0.25, -0.25, 0.25), new Point3D(-0.25, -0.25, -0.25),
              new Point3D(0.25, -0.25, -0.25), new Point3D(0.25, -0.25, 0.25) }
        };

        protected override Point3D[,] Faces
        {
            get
            {
                return faces;
            }
        }
    }
}
