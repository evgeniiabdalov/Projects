package Business;

import Statistics.*;
import Visualization.GraphVisual;

import java.util.List;
import java.util.Set;
import java.util.TreeMap;

public class GraphController {

    private Integer                            graphControllerID;

    private Graph                              graph;
    private GraphVisual                        graphVisual;

    private TreeMap<Integer, AgentController>  agentControllers;
    private AgentController                    currentAgentController;

    private AgentState                         agentState;

    private StatisticsData                     statisticsData;

    public                 GraphController( Integer graphControllerID, Graph graph ){

         this.graphControllerID = graphControllerID;

         this.graph = graph;

         this.agentControllers = new TreeMap<>();

         this.makeGraphVisual();

         this.agentState = new AgentState();

         this.currentAgentController = null;

    }

    public Integer         getGraphControllerID(){
        return this.graphControllerID;
    }

    public Integer         getLastAgentControllerID(){
        if( this.agentControllers.size() != 0 )
            return (this.agentControllers.size() - 1);
        else
            return 0;
    }

    public Graph           getGraph(){
           return this.graph;
    }

    public void            addAgentController(TreeMap<Integer, Agent> agentMap){

        Integer tmpID = this.agentControllers.size();

        AgentController agentController = new AgentController( tmpID, agentMap );

        this.agentControllers.put(tmpID, agentController);

        this.currentAgentController = agentController;

        this.makeCurrentAgentControllerStateData();

    }

    private void           makeGraphVisual(){

        graphVisual = new GraphVisual();

        TreeMap<Integer, Vertex> tmpVerticesMap = this.graph.getVerticesMap();

        for(Vertex vertex: this.graph.getVertices()){
            this.graphVisual.addCircle( vertex );
        }

        for(Edge edge: this.graph.getEdges()){

            Vertex fromVertex = tmpVerticesMap.get(edge.getFromIndex());
            Vertex toVertex   = tmpVerticesMap.get(edge.getToIndex());

            this.graphVisual.addLine(fromVertex, toVertex);

        }

    }

    public GraphVisual     getGraphVisual(){
        return this.graphVisual;
    }

    public boolean         setCurrentAgentController(Integer agentControllerID){

        this.currentAgentController = null;
        this.currentAgentController = this.agentControllers.get(agentControllerID);

        if(this.currentAgentController == null)
            return false;

        return true;
    }

    public AgentController getCurrentAgentController(){
        return this.currentAgentController;
    }

    public void            makeCurrentAgentControllerStatisticsData(){

        AgentTimeRatioData agentTimeRatioData = new AgentTimeRatioData();

        agentTimeRatioData.setTotalArrivedTime(this.getCurrentAgentController().getTotalArrivedTime());
        agentTimeRatioData.setTotalWaitingTime(this.getCurrentAgentController().getTotalWaitingTime());
        agentTimeRatioData.setTotalInitializingTime(this.getCurrentAgentController().getTotalInitializingTime());
        agentTimeRatioData.setTotalMovingTime(this.getCurrentAgentController().getTotalMovingTime());

        //--------------------------------------------------------------------------------

        MovementAnalyzer movementAnalyzer = new MovementAnalyzer();

        Set<Integer> agentIDs = this.getCurrentAgentController().getAngentIDs();

        for(Integer agentID : agentIDs){

            AgentTimeRatio agentTimeRatio = new AgentTimeRatio();

            agentTimeRatio.setMovingTime(this.getCurrentAgentController().getAgentMovingTime(agentID));
            agentTimeRatio.setWaitingTime(this.getCurrentAgentController().getAgentWaitingTime(agentID));
            agentTimeRatio.setInitializingTime(this.getCurrentAgentController().getAgentInitializingTime(agentID));
            agentTimeRatio.setArrivedTime(this.getCurrentAgentController().getAgentArrivedTime(agentID));

            System.out.println("moving " + this.getCurrentAgentController().getAgentMovingTime(agentID));
            System.out.println("waiting " + this.getCurrentAgentController().getAgentWaitingTime(agentID));
            System.out.println("initializing " + this.getCurrentAgentController().getAgentInitializingTime(agentID));
            System.out.println("arrived " + this.getCurrentAgentController().getAgentArrivedTime(agentID));


            agentTimeRatioData.setAgentTimeRatioTreeMap(agentID, agentTimeRatio);

            //----------------------------------------------------------------------------

            List<Step> stepList = this.getCurrentAgentController().getAgentPlan(agentID).getStepList();

            Boolean lastMovement = false;

            for(int i = 0; i < stepList.size(); i++) {

                if( i == stepList.size() - 1 )
                    lastMovement = true;

                Integer fromVertex = stepList.get(i).getFrom();
                Integer toVertex = stepList.get(i).getTo();


                movementAnalyzer.addAgentMovementData(agentID,
                                                      this.graph.getVerticesMap().get(fromVertex).getXCoordinate(),
                                                      this.graph.getVerticesMap().get(fromVertex).getYCoordinate(),
                                                      this.graph.getVerticesMap().get(toVertex).getXCoordinate(),
                                                      this.graph.getVerticesMap().get(toVertex).getYCoordinate(),
                                                      stepList.get(i).getTimeInterval().getStartTime(),
                                                      stepList.get(i).getTimeInterval().getFinishTime(),
                                                      this.getCurrentAgentController().getAgentRadius(agentID),
                                                      lastMovement
                                                     );

            }

        }

        movementAnalyzer.analyzeMovement();

        this.statisticsData = movementAnalyzer.getStatisticsData();
        this.statisticsData.setAgentTimeRatioData(agentTimeRatioData);


    }

    public StatisticsData  getGraphStatisticsData(){

        this.makeCurrentAgentControllerStatisticsData();

        return this.statisticsData;
    }

    private void           makeCurrentAgentControllerStateData(){

        Set<Integer> agentIDs = this.getCurrentAgentController().getAngentIDs();

        for(Integer agentID : agentIDs) {
            System.out.println("agent ID " + agentID);
            List<Step> stepList = this.getCurrentAgentController().getAgentPlan(agentID).getStepList();

            for(int i = 0; i < stepList.size(); i++) {


                Integer fromVertex = stepList.get(i).getFrom();
                Integer toVertex = stepList.get(i).getTo();

                if( fromVertex == toVertex ){
                    if(fromVertex == this.getCurrentAgentController().getAgentStartPosition(agentID))
                        this.getCurrentAgentController().setAgentState(agentID, stepList.get(i).getTimeInterval().getStartTime(), stepList.get(i).getTimeInterval().getFinishTime(), this.agentState.getStateIndex("INITIALIZING"));//initializing
                    else if(fromVertex == this.getCurrentAgentController().getAgentTargetPosition(agentID))
                        this.getCurrentAgentController().setAgentState(agentID, stepList.get(i).getTimeInterval().getStartTime(), stepList.get(i).getTimeInterval().getFinishTime(), this.agentState.getStateIndex("ARRIVED"));//arrived
                    else
                        this.getCurrentAgentController().setAgentState(agentID, stepList.get(i).getTimeInterval().getStartTime(), stepList.get(i).getTimeInterval().getFinishTime() , this.agentState.getStateIndex("WAITING"));//waiting
                }
                else if(toVertex == this.getCurrentAgentController().getAgentTargetPosition(agentID)) {
                    this.getCurrentAgentController().setAgentState(agentID, stepList.get(i).getTimeInterval().getStartTime(), stepList.get(i).getTimeInterval().getFinishTime(), this.agentState.getStateIndex("MOVING"));//moving
                    this.getCurrentAgentController().setAgentState(agentID, stepList.get(i).getTimeInterval().getFinishTime(), stepList.get(i).getTimeInterval().getFinishTime(), this.agentState.getStateIndex("ARRIVED"));//arrived
                }
                else
                    this.getCurrentAgentController().setAgentState(agentID, stepList.get(i).getTimeInterval().getStartTime(), stepList.get(i).getTimeInterval().getFinishTime() , this.agentState.getStateIndex("MOVING"));//moving

            }

        }

        this.getCurrentAgentController().makeTotalTime();

        System.out.println("---------------------------------------------");

    }

    public Integer         getAgentState(Integer agentIndex, Integer time){
        return this.currentAgentController.getAgentState(agentIndex, time);
    }


}
