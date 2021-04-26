package Statistics;

/**
 * SpaceTimeData class keeps data about where in time and space is an Agent - its Coordinates and Time values.
 * @author Evgenii Abdalov
 */
public class SpaceTimeData {


     private Integer      agentID;
     private Coordinate   space;
     private Double       time;


     public SpaceTimeData(Integer agentID, Coordinate space, Double time){

         this.agentID = agentID;
         this.space   = space;
         this.time    = time;

     }

    public Integer getAgentID() {
        return agentID;
    }

    public Coordinate getSpace() {
        return space;
    }

    public Double getTime() {
        return time;
    }

    public String toString(){

         String spaceTimeDataString = "Agent index " + this.agentID + " space " + this.space + " time " + this.time;

         return spaceTimeDataString;

    }

}
