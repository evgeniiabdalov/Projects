package Business;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.TreeMap;

/**
 * Graph class represents a graph on which Agents are moving
 * @author Evgenii Abdalov
 */
public class Graph {


    private  List<Edge>               edges;
    private  TreeMap<Integer, Vertex> verticesMap;

    public Graph(){
        this.edges = new ArrayList<>();
        this.verticesMap = new TreeMap<>();
    }

    public void addVertex(int index, double xCoordinate, double yCoordinate){
        this.verticesMap.put(index, new Vertex(index, xCoordinate, yCoordinate));
    }

    public void addEdge(int fromIndex, int toIndex){
        this.edges.add(new Edge(fromIndex, toIndex));
    }

    public List<Vertex> getVertices(){

        List<Vertex> vertices = new ArrayList<>();

        Set<Integer> verticesKey = this.verticesMap.keySet();

        for(Integer vertexKey: verticesKey){
             vertices.add(this.verticesMap.get(vertexKey));
        }

        return vertices;
    }

    public List<Edge> getEdges(){
        return this.edges;
    }

    public TreeMap<Integer, Vertex> getVerticesMap(){
        return this.verticesMap;
    }


    public Double getGraphDensity(){

        Double vertexAmount = new Double (this.verticesMap.size());
        Double edgeAmount = new Double (this.edges.size() );

        Double fullGraphEdgeAmount = vertexAmount * (vertexAmount - 1);

        Double graphDensity = vertexAmount / fullGraphEdgeAmount;

        return graphDensity;

    }

    public Integer getVertexAmount(){
        return this.verticesMap.size();
    }

    public Integer getEdgeAmount(){
        return this.edges.size();
    }

    @Override
    public String toString(){

        String mapString = "Map:\n";

        for(Vertex vertex : this.getVertices())
            mapString += vertex.toString();

        mapString += "---------------------------\n";

        for(Edge edge : this.edges)
            mapString += edge.toString();

        mapString += "---------------------------\n";

        mapString += "end map\n";

        return mapString;
    }

    public boolean equals(Graph anotherGraph) {

        List<Vertex> currentVertices = this.getVertices();
        List<Vertex> anotherVertices = anotherGraph.getVertices();

        if(this.edges.size() != anotherGraph.edges.size())
            return false;

        if(currentVertices.size() != anotherVertices.size())
            return false;

        for(Vertex thisVertex : currentVertices){
            for(Vertex anotherVertex: anotherVertices){
                if( !thisVertex.equals(anotherVertex) )
                    return false;
            }
        }

        for(Edge thisEdge : this.edges){
            for(Edge anotherEdge: anotherGraph.edges){
                if( !thisEdge.equals(anotherEdge) )
                    return false;
            }
        }

        return true;

    }

}
