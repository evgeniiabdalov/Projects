package Business;

/**
 * Step class represents a structural unit of an Agent movements on a Graph
 * @author Evgenii Abdalov
 */
public class Step {

    private TimeInterval timeInterval;
    private Integer      from;
    private Integer      to;

    public Step(TimeInterval timeInterval, int from, int to) {
        this.timeInterval = timeInterval;
        this.from = from;
        this.to = to;
    }


    public TimeInterval getTimeInterval() {
        return timeInterval;
    }

    public int getFrom() {
        return from;
    }

    public int getTo() {
        return to;
    }

    @Override
    public String toString(){

        String stepString = this.from + " --> " + this.to + " " + this.timeInterval.toString() + "\n";

        return stepString;

    }

}