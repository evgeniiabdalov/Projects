package Statistics;

import javafx.util.Pair;

import java.util.*;

/**
 * MovementAnalyzer class generate AgentMovementData for each Agent,
 * analyzes Agents movement, searching for collisions and calculating rate of collision risk between Agents
 * @author Evgenii Abdalov
 */
public class MovementAnalyzer {

     private TreeMap<Integer, AgentMovementData>                   agentMovementMap;
     private TreeMap<Integer, Double>                              agentRadiusMap;

     private CollisionData                                         collisionData;
     private StatisticsData                                        statisticsData;


    /**
     * MovementAnalyzer class constructor
     **/
     public                                 MovementAnalyzer(){

         this.agentMovementMap   = new TreeMap<>();
         this.agentRadiusMap   = new TreeMap<>();

         this.collisionData = new CollisionData();
         this.statisticsData = new StatisticsData();

     }

    /**
     * Creates AgentMovementData for an Agent for a specific time period of movement
     * @param agentID Agent unique identifier
     * @param x1 x coordinate of starting point
     * @param y1 y coordinate of starting point
     * @param y1 x coordinate of finish point
     * @param x2 y coordinate of finish point
     * @param startTime time of beginning of movement
     * @param finishTime time of ending of movement
     * @param agentRadius parameter is needed to detect collision event and calculate collision risk
     * @param lastMovement  if it is the last movement we should include the last point in SpaceTimeData collection
     **/
     public void                             addAgentMovementData(Integer agentID, Double x1, Double y1, Double x2, Double y2, Double startTime, Double finishTime, Double agentRadius, Boolean lastMovement){

             if( this.agentMovementMap.containsKey(agentID) )
                this.agentMovementMap.get(agentID).createAgentMovementData(x1, y1, x2, y2, startTime, finishTime, lastMovement);
             else
             {
                  AgentMovementData agentMovementData = new AgentMovementData(agentID);
                  agentMovementData.createAgentMovementData(x1, y1, x2, y2, startTime, finishTime, lastMovement);
                  this.agentMovementMap.put(agentID, agentMovementData);
                  this.agentRadiusMap.put(agentID, agentRadius);
             }

     }


     /**
     * Analyzes movement, checks for collision events, calculates rate of collision risk and calculates distances between Agents
      * All generated data is then added to StatisticsData object
     **/
     public void                            analyzeMovement() {

         //collision data
         List<Pair<Double, Double>>               collisionTimeList = new ArrayList<>();
         TreeMap<Integer, Pair<Integer, Integer>> collisionMap = new TreeMap<>();

         //distance data
         TreeMap<Integer, DistanceData> distanceDataMap = new TreeMap<>();

         //collision risk data
         TreeMap<Integer, CollisionRiskData> collisionRiskDataMap = new TreeMap<>();


         Integer generalTime = this.agentMovementMap.get(1).getAgentMovementTime();

         List<Integer> agentIDs = new ArrayList<>(this.agentMovementMap.keySet());

         for(int i = 0; i < agentIDs.size(); i++){

             for(int j = i; j < agentIDs.size(); j++){

                 if( i == j )
                     continue;

                 boolean collisionActive = false;
                 double  startTime = 0.0;

                 for(int currentTime = 0; currentTime < generalTime; currentTime++){

                    Double currentDistance = this.getDistance(this.agentMovementMap.get(agentIDs.get(i)).getSpaceTimeData(currentTime).getSpace(),
                                                              this.agentMovementMap.get(agentIDs.get(j)).getSpaceTimeData(currentTime).getSpace(),
                                                              this.agentRadiusMap.get(agentIDs.get(i)),
                                                              this.agentRadiusMap.get(agentIDs.get(j)));

                    if(distanceDataMap.containsKey(currentTime))
                          distanceDataMap.get(currentTime).addDistanceData(agentIDs.get(i), agentIDs.get(j), currentDistance);
                    else{

                         DistanceData distanceData = new DistanceData();
                         distanceData.addDistanceData(agentIDs.get(i), agentIDs.get(j), currentDistance);
                         distanceDataMap.put(currentTime, distanceData);

                    }

                    //-----------------------------------------------------------------------------

                    Double currentRiskCollision = this.getRiskCollision(currentDistance);
                     System.out.println("distance " + currentDistance + " risk " + currentRiskCollision);
                     if(collisionRiskDataMap.containsKey(currentTime))
                         collisionRiskDataMap.get(currentTime).addCollisionRiskData(agentIDs.get(i), agentIDs.get(j), currentRiskCollision);
                     else{

                         CollisionRiskData collisionRiskData = new CollisionRiskData();
                         collisionRiskData.addCollisionRiskData(agentIDs.get(i), agentIDs.get(j), currentRiskCollision);
                         collisionRiskDataMap.put(currentTime, collisionRiskData);

                     }


                    //-----------------------------------------------------------------------------

                    if(collisionActive && currentTime == generalTime - 1){

                         double finishTime = ((double)currentTime)/1000;
                         Pair<Double,Double> timePair = new Pair<>(startTime, finishTime);

                         collisionMap.put(collisionTimeList.size(), new Pair<Integer, Integer>(agentIDs.get(i), agentIDs.get(j)) );

                         collisionTimeList.add(timePair);

                     }

                    if( this.checkCollision(this.agentMovementMap.get(agentIDs.get(i)).getSpaceTimeData(currentTime).getSpace(),
                                             this.agentMovementMap.get(agentIDs.get(j)).getSpaceTimeData(currentTime).getSpace(),
                                             this.agentRadiusMap.get(agentIDs.get(i)),
                                             this.agentRadiusMap.get(agentIDs.get(j))
                                            )
                    )
                    {
                         if(!collisionActive) {

                             collisionActive = true;
                             startTime = ((double)currentTime)/1000;

                         }

                     }
                    else{

                         if(collisionActive){

                             collisionActive = false;

                             double finishTime = ((double)currentTime)/1000;
                             Pair<Double,Double> timePair = new Pair<>(startTime, finishTime);

                             collisionMap.put(collisionTimeList.size(), new Pair<Integer, Integer>(agentIDs.get(i), agentIDs.get(j)) );

                             collisionTimeList.add(timePair);

                         }

                     }


                 }

             }
         }

         this.collisionData.setCollisionTimeList(collisionTimeList);
         this.collisionData.setCollisionTimeMap(collisionMap);

         this.statisticsData.setMovementData(this.agentMovementMap);
         this.statisticsData.setCollisionData(this.collisionData);
         this.statisticsData.setDistanceDataMap(distanceDataMap);
         this.statisticsData.setRiskDataMap(collisionRiskDataMap);

     }

    /**
     * Checks if collision event has happened at a certain moment of time at a certain Location between two Agents
     * @param firstLocation coordinates center of an Agent during a certain moment of time
     * @param secondLocation coordinates center of an Agent during a certain moment of time
     * @param firstRadius radius of an Agent
     * @param secondRadius radius of an Agent
     * @return true if collision event has happened, false if not
     **/
    private boolean                        checkCollision(Coordinate firstLocation, Coordinate secondLocation, Double firstRadius, Double secondRadius){

         Double distX = (secondLocation.getXCoordinate() - firstLocation.getXCoordinate());
         Double distY = (secondLocation.getYCoordinate() - firstLocation.getYCoordinate());

         Double distanceAgents = Math.sqrt(distX*distX + distY*distY);

         if( distanceAgents <= firstRadius + secondRadius)
             return true;

         return false;

     }

    /**
     * Calculates distance between two Agents at a certain moment of time at a certain Location
     * @param firstLocation coordinates center of an Agent during a certain moment of time
     * @param secondLocation coordinates center of an Agent during a certain moment of time
     * @param firstRadius radius of an Agent
     * @param secondRadius radius of an Agent
     * @return Double distance value between two Agents
     **/
    private Double                         getDistance(Coordinate firstLocation, Coordinate secondLocation, Double firstRadius, Double secondRadius){

         Double distX = (secondLocation.getXCoordinate() - firstLocation.getXCoordinate());
         Double distY = (secondLocation.getYCoordinate() - firstLocation.getYCoordinate());

         Double distanceAgents = Math.sqrt(distX*distX + distY*distY);

         distanceAgents -= (firstRadius + secondRadius);

         if(distanceAgents < 0.0)
             distanceAgents = 0.0;

         return distanceAgents;

     }

    /**
     * Calculates rate of collision risk based on distance between two Agents at a certain moment of time at a certain Location
     * @param  coordinates center of an Agent during a certain moment of time
     * @return Double rate of collision risk value between two Agents
     **/
    private Double                         getRiskCollision(Double distance){

         Double distanceDouble = distance * 100;

         Double collisionRisk = (1/distanceDouble) * 1000;

         return collisionRisk;

     }

     //-------------------------------------------------------------------------------

     public void                            printAgentMovement(Integer agentIndex){

        this.agentMovementMap.get(agentIndex).printMovementData();
        System.out.println("-----------------------------------------");
        System.out.println("-----------------------------------------");
        System.out.println("-----------------------------------------");

    }

     public CollisionData                   getCollisionData(){
         return this.collisionData;
     }

    /**
     * Return StatisticsData object to business logic module
     * @return StatisticsData object that contains all calculated statistical results
     **/
     public StatisticsData                  getStatisticsData(){
         return this.statisticsData;
     }

}
