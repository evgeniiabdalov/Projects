package Statistics;

import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;

/**
 * AgentMovementData class keeps data about Agent movement,
 * List<SpaceTimeData> agentMovement - keeps data for each millisecond of movement
 * (agentMovement.size() == number of milliseconds)
 * @author Evgenii Abdalov
 */
public class AgentMovementData {

    private Integer                               agentID;


    /**
     *  List<SpaceTimeData> agentMovement keeps data about Location in Time for an Agent,
     *  index of List value corresponds to an Integer value of time(millisecond),
     *  for example agentMovement.get(537) means it gets a SpaceTimeData object at 0.537 second of movement
     **/
    private List<SpaceTimeData>                   agentMovement;

    private TreeMap<Integer, VectorData>          vectorDataMap;

    private TreeMap<Integer, Double>              speedMovement;


    /**
     * AgentMovementData class constructor
     * @param agentID Agent unique identifier
     **/
    public AgentMovementData(Integer agentID){

        this.agentID = agentID;

        this.agentMovement = new ArrayList<>();
        this.speedMovement = new TreeMap<>();

    }

    /**
     * Returns an Integer value Agent identification
     * @return Integer Agent unique identifier
     **/
    public  Integer                         getAgentIndex(){
        return this.agentID;
    }

    /**
     * Creates collection of SpaceTimeData objects for each millisecond of movement
     * @param x1 x coordinate of starting point
     * @param y1 y coordinate of starting point
     * @param x2 x coordinate of finish point
     * @param y2 y coordinate of finish point
     * @param startTime time of beginning of movement
     * @param finishTime time of ending of movement
     * @param lastMovement if it is the last movement we should include the last point in SpaceTimeData collection
     **/
    public  void                            createAgentMovementData(Double x1, Double y1, Double x2, Double y2,  Double startTime, Double finishTime, boolean lastMovement){


        double startTimeDouble = startTime;
        double finishTimeDouble = finishTime;

        int  startTimeInt  = (int)(startTimeDouble*1000);
        int  finishTimeInt = (int)(finishTimeDouble*1000);
        int  deltaTimeInt  = finishTimeInt - startTimeInt;

        double deltaTimeDouble  = finishTime - startTime;

        //--------------------------------------------------------------------

        VectorData currentVectorData = new VectorData(x1, y1, x2, y2);

        Double agentSpeed = currentVectorData.getModule() / deltaTimeDouble;

       for( int momentTimeInt = 0; momentTimeInt < deltaTimeInt; momentTimeInt++ ){

            this.speedMovement.put(this.agentMovement.size(), agentSpeed);

            double momentTimeDouble = ((double)momentTimeInt/1000);

                Double currentModule = agentSpeed * momentTimeDouble;

                Coordinate currentCoordinate = this.getCurrentCoordinates(new Coordinate(x1,y1), currentVectorData.getDirectionVector(), currentVectorData.getModule(), currentModule);

                this.agentMovement.add(new SpaceTimeData(this.agentID, currentCoordinate, startTime + momentTimeDouble));

        }

        if(lastMovement) {
            this.speedMovement.put(this.agentMovement.size(), agentSpeed);
            this.agentMovement.add(new SpaceTimeData(this.agentID, new Coordinate(x2, y2), startTime + deltaTimeDouble));
        }



    }

    /**
     * Based on start vector, direction and distance calculates current coordinates
     * @param start start vector of movement
     * @param direction direction of vector of movement
     * @param mainModule distance between start point and finish point of movement
     * @param currentModule distance between start point and current point of movement
     * @return Coordinate current coordinates at a certain moment of time
     **/
    private Coordinate                      getCurrentCoordinates(Coordinate start, Coordinate direction, Double mainModule, Double currentModule){

        if( mainModule == 0 )
            return new Coordinate(start.getXCoordinate(), start.getYCoordinate());

        Double ratio = currentModule / mainModule;

        Double deltaX = direction.getXCoordinate() * ratio;
        Double deltaY = direction.getYCoordinate() * ratio;

        Double currentX = start.getXCoordinate() + deltaX;
        Double currentY = start.getYCoordinate() + deltaY;

        return new Coordinate(currentX, currentY);

    }

    /**
     * Returns SpaceTimeData that corresponds to a certain moment of time
     * @param time moment of time
     * @return SpaceTimeData object that contains Location in Time
     **/
    public  SpaceTimeData                   getSpaceTimeData(Integer time){
        return this.agentMovement.get(time);
    }

    /**
     * Returns total number of milliseconds in movement
     * @return an Integer value of total time
     **/
    public  Integer                         getAgentMovementTime(){
        return this.agentMovement.size();
    }

    /**
     * Returns current speed at a certain moment of time
     * @return TreeMap<Integer, Double> where a key is an Integer value of time moment and a value is Double speed of movement
     **/
    public  TreeMap<Integer, Double>        getSpeedMovement(){
        return this.speedMovement;
    }

    public  void                            printMovementData(){

        for(int i = 0; i < this.agentMovement.size(); i++){
            System.out.println(agentMovement.get(i));
        }

    }




}
