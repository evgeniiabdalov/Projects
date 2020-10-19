package Statistics;

import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;


public class AgentMovementData {

    private Integer                               agentIndex;


    private List<SpaceTimeData>                   agentMovement;
    private TreeMap<Integer, VectorData>          vectorDataMap;

    private TreeMap<Integer, Double>              speedMovement;


    public AgentMovementData(Integer agentIndex){

        this.agentIndex = agentIndex;

        this.agentMovement = new ArrayList<>();
        this.speedMovement = new TreeMap<>();

    }

    public  Integer                         getAgentIndex(){
        return this.agentIndex;
    }

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

                this.agentMovement.add(new SpaceTimeData(this.agentIndex, currentCoordinate, startTime + momentTimeDouble));

        }

        if(lastMovement) {
            this.speedMovement.put(this.agentMovement.size(), agentSpeed);
            this.agentMovement.add(new SpaceTimeData(this.agentIndex, new Coordinate(x2, y2), startTime + deltaTimeDouble));
        }



    }

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

    public  SpaceTimeData                   getSpaceTimeData(Integer time){
        return this.agentMovement.get(time);
    }

    public  Integer                         getAgentMovementTime(){
        return this.agentMovement.size();
    }

    public  TreeMap<Integer, Double>        getSpeedMovement(){
        return this.speedMovement;
    }

    public  void                            printMovementData(){

        for(int i = 0; i < this.agentMovement.size(); i++){
            System.out.println(agentMovement.get(i));
        }

    }




}
