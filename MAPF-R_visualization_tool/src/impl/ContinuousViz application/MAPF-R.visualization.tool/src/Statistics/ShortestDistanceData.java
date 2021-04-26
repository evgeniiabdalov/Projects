package Statistics;

import Business.*;

import java.util.*;

/**
 * ShortestDistanceData class calculates shortest path distances using BFS algorithm and compares it
 * with an actual path distance, if an actual path distance is greater than shortest distance, then
 * path is not optimal
 * @author Evgenii Abdalov
 */
public class ShortestDistanceData {

    /**
    * List<Integer> notShortestDistanceList keeps data about Agent with non-optimal paths,
     * where an Integer value in an Agent unique identifier
    */
    private List<Integer>                               notShortestDistanceList;

    private LinkedList<Integer>                         adjacentList[];
    private TreeMap<Integer, Integer>                   levelVertexMap;


    private static Graph                    graph;
    private static TreeMap<Integer, Agent>  agentMap;

    /**
     * ShortestDistanceData class constructor
     * @param graph Graph where movement is being processed
     * @param agentMap collection of Agents, for which calculations are being processed
     **/
    public ShortestDistanceData(Graph graph, TreeMap<Integer, Agent> agentMap){

        this.graph    = graph;
        this.agentMap = agentMap;

        this.adjacentList = new LinkedList[this.graph.getVertexAmount()];

        for( int i = 0; i < this.graph.getVertexAmount(); i++ ){
            this.adjacentList[i] = new LinkedList<>();
        }

        this.levelVertexMap = new TreeMap<>();

        this.notShortestDistanceList = new ArrayList<>();

        this.runDistanceTest();

    }

    /**
     * Creates data for BFS algorithm calculations
     **/
    private void createBFSData(){

         List<Edge> edgesList = this.graph.getEdges();
         System.out.println("creating bfs data");
         for(Edge edge : edgesList){
             this.adjacentList[edge.getFromIndex()].add(edge.getToIndex());
             this.adjacentList[edge.getToIndex()].add(edge.getFromIndex());
         }

    }

    /**
     * Returns result of BFS algorithm calculations for two Vertices
     * @param startVertex start Vertex unique identifier
     * @param finishVertex finish Vertex unique identifier
     * @return Double shortest distance value between two Vertices
     **/
    private Double getBFSDistance(int startVertex, int finishVertex){

        this.createBFSData();

        int initialVertex = startVertex;

        //-------------------------------------------------------------------------------

        boolean visited[] = new boolean[this.graph.getVertexAmount()];

        LinkedList<Integer> queue = new LinkedList<>();

        visited[startVertex] = true;
        queue.add(startVertex);
        this.levelVertexMap.put(startVertex, startVertex);

        while(true){

            if(queue.size() == 0)
                break;

            startVertex = queue.poll();
            //System.out.println(startVertex + " ");


            if(startVertex == finishVertex)
                break;

            Iterator<Integer> vertexIterator = this.adjacentList[startVertex].listIterator();

            while( vertexIterator.hasNext() ){

                int nextVertex = vertexIterator.next();

                if( !visited[nextVertex] ){

                    visited[nextVertex] = true;
                    queue.add(nextVertex);
                    this.levelVertexMap.put(nextVertex, startVertex);

                }

            }

        }


        Double bfsDistance = 0.0;


        while(true){

            if(finishVertex == initialVertex)
                break;

            Integer previousVertex = this.levelVertexMap.get(finishVertex);

            bfsDistance += this.assessDistance(finishVertex, previousVertex);

            System.out.println(previousVertex + " " + finishVertex + ": distance " + this.assessDistance(finishVertex, previousVertex));

            finishVertex = previousVertex;


        }


        return bfsDistance;


    }

    /**
     * Returns an actual distance according to an Agent Steps during movement
     * @param currentSteps List of Steps that an Agent takes during movement to get
     * from start Vertex to finish Vertex
     * @return Double distance value an Agent takes to get from start Vertex to finish Vertex
     **/
    private Double getActualDistance(List<Step> currentSteps){

        Double actualDistance = 0.0;

        for(Step step : currentSteps){
           actualDistance += this.assessDistance(step.getFrom(), step.getTo());
        }

        return actualDistance;

    }

    /**
     * Returns a distance between two Vertices
     * @param firstVertexIndex first Vertex unique identifier
     * @param secondVertexIndex
     * @return Double distance value an Agent takes to get from start Vertex to finish Vertex
     **/
    private Double assessDistance(Integer firstVertexIndex, Integer secondVertexIndex){

        TreeMap<Integer, Vertex> verticesMap = this.graph.getVerticesMap();

        Vertex firstVertex  = verticesMap.get(firstVertexIndex);
        Vertex secondVertex = verticesMap.get(secondVertexIndex);


        VectorData currentVector = new VectorData(
                firstVertex.getXCoordinate(), firstVertex.getYCoordinate(),
                secondVertex.getXCoordinate(), secondVertex.getYCoordinate()
        );

        return currentVector.getModule();

    }

    /**
     * Runs the whole calculation process and creates notShortestDistanceList
     **/
    private void runDistanceTest(){

        Double actualDistance = 0.0;
        Double bfsDistance    = 0.0;

        Set<Integer> agentKeys = this.agentMap.keySet();

        for(Integer agentKey : agentKeys){

            System.out.println("AGENT " + agentKey);

            List<Step> currentSteps = this.agentMap.get(agentKey).getPlan().getStepList();

            Integer startVertex = currentSteps.get(0).getFrom();
            Integer finishVertex = currentSteps.get(currentSteps.size() - 1).getTo();

            actualDistance = this.getActualDistance(currentSteps);
            bfsDistance    = this.getBFSDistance(startVertex, finishVertex);


            System.out.println("actualDistance " + actualDistance);
            System.out.println("bfsDistance " + bfsDistance);

            if( !bfsDistance.equals(actualDistance) ){
                this.notShortestDistanceList.add(agentKey);
            }
        }


    }

    /**
     * Returns List of Agents that have non-optimal path distances
     * @return List<Integer> where an Integer value is an Agent unique identifier
     **/
    public List<Integer> getNotShortestDistanceList(){
        return this.notShortestDistanceList;
    }



}
