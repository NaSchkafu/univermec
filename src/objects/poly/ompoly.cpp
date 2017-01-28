#include "ompoly.hpp"

// Fix fuer OpenMesh
namespace core
{
  namespace arith
  {
    inline bool operator<(const core::arith::interval &i, double i2)
    {
      if(sup(i) < i2)
	return true;
      else if(inf(i) > i2)
	return false;
      assert(!"Openmesh operation not supported with intervals!");
      throw "INVALID Operation";
    }
  }
}

namespace objects
{
  namespace io
  {
    using namespace core::arith;

    OpenMeshPolyhedron<mreal>* load_off(const char* filename)
    {
      std::ifstream f;
      f.open(filename);
      OpenMeshPolyhedron<mreal>* result = load_off(f);
      f.close();
      return result;
    }

    OpenMeshPolyhedron<mreal>* load_off(std::istream& in)
    {
      typedef Mesh_Types<mreal>::Mesh Mesh;
      char off[4];
      in.getline(off, 4);
      if( strcmp(off, "OFF") )
      {
        throw FileLoaderException();
      }
      int num_vert, num_faces, num_edges;
      in >> num_vert >> num_faces >> num_edges;
      //std::cout << num_vert << num_faces << num_edges << std::endl;

      Mesh mesh;
      // Read the vertices.
      std::vector<Mesh::VertexHandle> vhandles;
      for(int i=0; i < num_vert; ++i)
      {
        double x, y, z;
        in >> x >> y >> z;
        Mesh::Point p(x, y, z);
        vhandles.push_back(mesh.add_vertex(p));
      }

      // Read the faces
      std::vector<Mesh::VertexHandle> face_vhandles;
      for(int i = 0; i < num_faces; ++i)
      {
        int v;
        in >> v;
        face_vhandles.clear();

        // Read vertex indices for polygon with v vertices
        while(v > 0)
        {
          v--;
          int index;
          in >> index;
          face_vhandles.push_back( vhandles[index] );
        }
        //std::cout << std::endl;
        mesh.add_face(face_vhandles);
      }
      return new OpenMeshPolyhedron<mreal>(mesh);
    }

    OpenMeshPolyhedron<interval>* load_interval_off(const char* filename)
    {
      std::ifstream f;
      f.open(filename);
      OpenMeshPolyhedron<interval>* result = load_interval_off(f);
      f.close();
      return result;
    }

    OpenMeshPolyhedron<interval>* load_interval_off(std::istream& in)
    {
      typedef Mesh_Types<interval>::Mesh Mesh;
      char off[6];
      in.getline(off, 6);
      if( strcmp(off, "IEOFF") ) // Interval Extended Object File Format
      {
        throw FileLoaderException();
      }
      int num_vert, num_faces, num_edges;
      in >> num_vert >> num_faces >> num_edges;
      //std::cout << num_vert << num_faces << num_edges << std::endl;

      Mesh mesh;
      // Read the vertices.
      std::vector<Mesh::VertexHandle> vhandles;
      for(int i=0; i < num_vert; ++i)
      {
        double lx, ux, ly, uy, lz, uz;
        in >> lx >> ux >> ly >> uy >> lz >> uz;
        Mesh::Point p(interval(lx, ux), interval(ly, uy), interval(lz, uz));
        vhandles.push_back(mesh.add_vertex(p));
      }

      // Read the faces
      std::vector<Mesh::VertexHandle> face_vhandles;
      for(int i = 0; i < num_faces; ++i)
      {
        int v;
        in >> v;
        face_vhandles.clear();

        // Read vertex indices for polygon with v vertices
        while(v > 0)
        {
          v--;
          int index;
          in >> index;
          face_vhandles.push_back( vhandles[index] );
        }
        //std::cout << std::endl;
        mesh.add_face(face_vhandles);
      }
      return new OpenMeshPolyhedron<interval>(mesh);
    }


    OpenMeshPolyhedron<core::arith::mreal>* load_obj(const char* filename)
    {
      typedef Mesh_Types<mreal>::Mesh Mesh;
      std::ifstream f;
      std::vector<Mesh::VertexHandle> vhandles;
      std::vector<Mesh::VertexHandle> face_vhandles;
      Mesh mesh;
      f.open(filename);
      while( true )
      {
        // Read the file line by line
        char line[256];
        f.getline(line, 256);
        if( !f.good() )
        {
          break;
        }
        //std::cout << line << std::endl;

        // Search for comments ( everything after #) and truncate the string
        // char* comment = strchr(line, '#');
        /*
        if(comment != NULL)
        {
        std::cout << *comment << std::endl;
        *comment = 0;
        }
        */
        // "Command" handling.
        char* cmd = strtok(line, " ");
        //std::cout << strlen(cmd) << " " << cmd << std::endl;
        if( !strcmp(cmd, "v") )
        {
          //This is a vertex
          //std::cout << "Vertex" << std::endl;
          double x, y, z;
          x = boost::lexical_cast<double>( strtok(NULL, " "));
          y = boost::lexical_cast<double>( strtok(NULL, " "));
          z = boost::lexical_cast<double>( strtok(NULL, " "));
          Mesh::Point p(x, y, z);
          vhandles.push_back(mesh.add_vertex(p));
        }
        else if( !strcmp( cmd, "f") )
        {
          // This is a face (with a unknown number of vertices
          // each vertex is defined with v/vt/vn (vertex/texture/normal) (vt and vn are optional)
          // std::cout << "Face" << std::endl;
          std::vector<char*> vertices;
          char* v;
          while( ( v = strtok(NULL, " ")) != NULL )
          {
            vertices.push_back(v);
          }
          face_vhandles.clear();
          // Now the v/vt/vn tuples are broken down (we need only v now)
          for( std::vector<char*>::iterator i = vertices.begin(); i != vertices.end(); ++i )
          {
            int index = atoi( strtok(*i, "/") );
            face_vhandles.push_back(vhandles[index-1]);
          }
          mesh.add_face(face_vhandles);
        }
        //std::cout << "Next line..." << std::endl;
      }
      return new OpenMeshPolyhedron<mreal>(mesh);
    }


    namespace details
    {
      std::string off_vertex(const Mesh_Types<mreal>::Vector &v)
      {
        std::stringstream ss;
        ss << v[0] << " " << v[1] << " " << v[2];
        return ss.str();
      }

      std::string off_vertex(const Mesh_Types<interval>::Vector &v)
      {
        std::stringstream ss;
        ss << inf(v[0]) << " " << sup(v[0]) << " " << inf(v[1]) << " " << 
	  sup(v[1]) << " " << inf(v[2]) << " " << sup(v[2]);
        return ss.str();
      }
    }
  } //namespace io
} //namespace objects

