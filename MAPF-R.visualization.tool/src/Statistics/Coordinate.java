package Statistics;

public class Coordinate {

    private double xCoordinate;
    private double yCoordinate;

    public  Coordinate(){
        this.xCoordinate = 0.0;
        this.yCoordinate = 0.0;
    }

    public Coordinate( double x, double y ){
        this.xCoordinate = x;
        this.yCoordinate = y;
    }

    public double getXCoordinate() {
        return xCoordinate;
    }

    public double getYCoordinate() {
        return yCoordinate;
    }

    @Override
    public String toString() {
        String coordinateString = "x: " + this.getXCoordinate() + " y: " + this.getYCoordinate();
        return coordinateString;
    }
}
