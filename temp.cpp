// Define a struct for a vertex
struct Vertex
{
    double x, y;            // Coordinates of the vertex
    HalfEdge *incidentEdge; // Pointer to an incident half-edge
};

// Define a struct for a half-edge
struct HalfEdge
{
    Vertex *origin;     // Pointer to the origin vertex
    HalfEdge *twin;     // Pointer to the twin half-edge
    HalfEdge *next;     // Pointer to the next half-edge
    HalfEdge *prev;     // Pointer to the previous half-edge
    Face *incidentFace; // Pointer to an incident face
    Edge *parentEdge;   // Pointer to the parent edge
};

// Define a struct for an edge
struct Edge
{
    HalfEdge *edge1; // Pointer to the first half-edge of the edge
    HalfEdge *edge2; // Pointer to the second half-edge of the edge
};

// Define a struct for a face
struct Face
{
    HalfEdge *outerComponent; // Pointer to the outer component half-edge
};

// Function to link two vertices with an edge
void linkVertices(Vertex *v1, Vertex *v2)
{
    // Create a new edge
    Edge *e = new Edge;
    // Create two half-edges
    HalfEdge *he1 = new HalfEdge;
    HalfEdge *he2 = new HalfEdge;
    // Set the origin and twin pointers of the half-edges
    he1->origin = v1;
    he2->origin = v2;
    he1->twin = he2;
    he2->twin = he1;
    // Set the next and previous pointers of the half-edges
    he1->next = he2;
    he1->prev = he2;
    he2->next = he1;
    he2->prev = he1;
    // Set the incident edge pointers of the origin vertices
    v1->incidentEdge = he1;
    v2->incidentEdge = he2;
    // Set the parent edge pointer of the half-edges
    he1->parentEdge = e;
    he2->parentEdge = e;
}
