package Statistics;

public class VectorData {

    private Coordinate vectorStart;
    private Coordinate vectorFinish;
    private Coordinate vectorDirection;

    private Double     module;

    public VectorData(Double x1, Double y1, Double x2, Double y2){

        this.vectorStart  = new Coordinate(x1, y1);
        this.vectorFinish = new Coordinate(x2, y2);

        this.vectorDirection = this.subtractCoordinates(this.vectorStart, this.vectorFinish);

        this.module = this.assessModule(this.vectorDirection);

    }

    private Coordinate subtractCoordinates(Coordinate start, Coordinate finish){

        Double deltaX = finish.getXCoordinate() - start.getXCoordinate();
        Double deltaY = finish.getYCoordinate() - start.getYCoordinate();

        return new Coordinate(deltaX, deltaY);

    }

    private Double assessModule(Coordinate direction){
        return (Math.sqrt(direction.getXCoordinate()*direction.getXCoordinate() + direction.getYCoordinate()*direction.getYCoordinate()));
    }

    public Coordinate getDirectionVector() {
        return this.vectorDirection;
    }

    public Double getModule() {
        return module;
    }
}
