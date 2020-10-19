package Business;

/**
 *
 * @author abdalevg
 */
public class Edge {

    private Integer fromIndex;
    private Integer toIndex;

    public Edge(Integer fromIndex, Integer toIndex) {
        this.fromIndex = fromIndex;
        this.toIndex = toIndex;
    }

    public int getFromIndex() {
        return fromIndex;
    }

    public int getToIndex() {
        return toIndex;
    }

    public String toString(){

        String edgeString = "{" + fromIndex + "," + toIndex + "}" + "\n";

        return edgeString;

    }


    public boolean equals(Edge anotherEdge) {

        if(this.fromIndex != anotherEdge.fromIndex)
            return false;
        if(this.toIndex != anotherEdge.toIndex)
            return false;

        return true;

    }
}