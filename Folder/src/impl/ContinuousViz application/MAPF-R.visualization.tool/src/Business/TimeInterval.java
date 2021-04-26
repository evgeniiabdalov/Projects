package Business;

/**
 * TimeInterval class represents delta between start time moment of movement and finish time moment of movement
 * @author Evgenii Abdalov
 */
public class TimeInterval {

    private Double startTime;
    private Double finishTime;

    public TimeInterval(double startTime, double finishTime) {
        this.startTime = startTime;
        this.finishTime = finishTime;
    }

    public Double getStartTime() {
        return startTime;
    }

    public Double getFinishTime() {
        return finishTime;
    }

    public Double getValue(){
        return ( finishTime - startTime );
    }

    @Override
    public String toString(){

        String timeIntervalString = "Interval: [" + this.startTime + ", " + this.finishTime + "]";

        return timeIntervalString;

    }

}

