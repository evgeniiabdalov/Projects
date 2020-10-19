package Business;

/**
 *
 * @author abdalevg
 */
public class Vertex {

    private Integer vertexIndex;
    private Double  xCoordinate;
    private Double  yCoordinate;

    public Vertex(int vertexIndex, double xCoordinate, double yCoordinate){
        this.vertexIndex = vertexIndex;
        this.xCoordinate = xCoordinate;
        this.yCoordinate = yCoordinate;
    }

    public Integer getIndex() {
        return this.vertexIndex;
    }

    public Double getXCoordinate() {
        return xCoordinate;
    }

    public Double getYCoordinate() {
        return yCoordinate;
    }

    @Override
    public String toString(){

        String vertexString = vertexIndex + " : " + xCoordinate + " , " + yCoordinate + "\n";;

        return vertexString;
    }


    public boolean equals(Vertex anotherVertex) {

        if(this.vertexIndex != anotherVertex.vertexIndex)
               return false;
        if(this.xCoordinate != anotherVertex.xCoordinate)
               return false;
        if(this.yCoordinate != anotherVertex.yCoordinate)
               return false;

        return true;

    }
}