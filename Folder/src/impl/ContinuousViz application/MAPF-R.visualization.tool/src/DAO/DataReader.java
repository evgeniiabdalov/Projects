
package DAO;

import Business.Agent;
import Business.Graph;
import Business.Plan;
import Business.TimeInterval;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.TreeMap;

/**
 * DataReader class reads data from input files
 * @author Evgenii Abdalov
 */
public class DataReader {

    /**
     * Reads data from input file and creates an instance of Graph object
     * @param path input file address and name
     * @return Graph object
     **/
    public Graph readGraph(String path) throws FileNotFoundException{

        File    file  = new File(path);
        Scanner input = new Scanner(file);
        Graph   map   = new Graph();

        int     vertexIndex = 0;
        double  xCoordinate = 0;
        double  yCoordinate = 0;
        int     fromIndex   = 0;
        int     toIndex     = 0;


        int     locationAmount = 0;
        int     vertexAmount   = 0;
        int     edgeAmount     = 0;

        int     locationCounter = 0;
        int     edgeCounter     = 0;


        boolean readingError = false;

        boolean locationAmountSignal = false;
        boolean vertexAmountSignal   = false;
        boolean edgeAmountSignal     = false;


        boolean locationAmountCheck = false;
        boolean vertexAmountCheck   = false;
        boolean edgeAmountCheck     = false;

        boolean locationSubstringCheck = true;
        boolean vertexSubstringCheck   = true;
        boolean edgeSubstringCheck     = true;



        while(input.hasNext()){

            String nextLine =  input.next();
            System.out.println(nextLine);

            if( locationSubstringCheck ){

                if( nextLine.contains("Locations:") ){

                    locationAmountCheck    = true;
                    locationSubstringCheck = false;
                    locationAmountSignal   = true;
                    continue;

                }
            }
            if(  vertexSubstringCheck ){

                if( nextLine.contains("Vertices:") ){

                    vertexAmountCheck    = true;
                    vertexSubstringCheck = false;
                    vertexAmountSignal   = true;
                    continue;

                }
            }
            if( edgeSubstringCheck  ){

                if( nextLine.contains("Edges:") ){

                    edgeAmountCheck     = true;
                    edgeSubstringCheck  = false;
                    edgeAmountSignal    = true;
                    continue;

                }
            }



            if( locationAmountCheck ){

                try {
                    locationAmount = Integer.parseInt(nextLine);
                    locationAmountCheck = false;
                    continue;
                }
                catch(NumberFormatException ex){
                    readingError = true;
                    break;
                }

            }
            if( vertexAmountCheck ){


                try{
                    vertexAmount = Integer.parseInt(nextLine);
                    vertexAmountCheck = false;
                    continue;
                }
                catch(NumberFormatException ex){
                    readingError = true;
                    break;
                }


            }
            if( edgeAmountCheck ){

                try {
                    edgeAmount = Integer.parseInt(nextLine);
                    edgeAmountCheck = false;
                    continue;
                }
                catch(NumberFormatException ex){
                    readingError = true;
                    break;
                }

            }


            if( !locationSubstringCheck && vertexSubstringCheck && edgeSubstringCheck ){

                if( nextLine.contains(":") ){

                    try {
                        vertexIndex = Integer.parseInt(nextLine.substring(0, nextLine.indexOf(":")));
                    }
                    catch(NumberFormatException ex){
                        readingError = true;
                        break;
                    }

                }
                else if( nextLine.contains(",") ){

                    try {
                        xCoordinate = Double.parseDouble(nextLine.substring(0, nextLine.indexOf(",")));
                    }
                    catch(NumberFormatException ex){
                        readingError = true;
                        break;
                    }

                }
                else{

                    try {

                        yCoordinate = Double.parseDouble(nextLine);

                    }
                    catch(NumberFormatException ex){
                        readingError = true;
                        break;
                    }


                    map.addVertex( vertexIndex, xCoordinate, yCoordinate);

                    locationCounter++;

                }

            }

            if( !locationSubstringCheck && !vertexSubstringCheck && !edgeSubstringCheck ){

                try {
                    fromIndex = Integer.parseInt(nextLine.substring(nextLine.indexOf("{") + 1, nextLine.indexOf(",")));
                    toIndex = Integer.parseInt(nextLine.substring(nextLine.indexOf(",") + 1, nextLine.indexOf("}")));
                }
                catch(NumberFormatException ex){
                    readingError = true;
                    break;
                }

                map.addEdge(fromIndex, toIndex);


                edgeCounter++;
            }


        }

        System.out.println("Location counter: " + locationCounter);
        System.out.println("Edge counter: " + edgeCounter);

        if( locationAmount != locationCounter )
            readingError = true;

        if( edgeCounter != edgeAmount )
            readingError = true;


        if( readingError || !locationAmountSignal || !vertexAmountSignal || !edgeAmountSignal ) {
            System.out.println("flush");
            map.getVerticesMap().clear();
        }


        return map;

    }

    /**
     * Reads data from input file and creates a collection of Agents
     * @param path input file address and name
     * @return TreeMap<Integer, Agent> where an Integer value is a key identifier and a value is an Agent object  with this identifier
     **/
    public TreeMap<Integer, Agent> readAgents(String path) throws FileNotFoundException{


        File    file  = new File(path);
        Scanner input = new Scanner(file);

        TreeMap<Integer, Agent> agentMap = new TreeMap<>();


        boolean readingError = false;

        boolean agentCheck     = false;
        boolean startCheck    = false;
        boolean goalCheck     = false;
        boolean dataCheck     = false;


        int     agentIndex    = 0;
        int     graphIndex    = 0;

        String  agentDataString = "";

        while(input.hasNext()){

            String nextLine = input.next();
            //System.out.println(nextLine);

            if( !agentCheck && !startCheck && !goalCheck ){

                if( nextLine.contains("Kruhobots") ){
                    agentCheck = true;
                    continue;
                }

            }


            if( agentCheck && !startCheck && !goalCheck ){

                if( nextLine.contains(":") ){

                    try {
                        agentIndex = Integer.parseInt(nextLine.substring(0, nextLine.indexOf(":")));
                        agentMap.put(agentIndex, new Agent(agentIndex));
                    }
                    catch (NumberFormatException ex){
                        readingError = true;
                        break;
                    }

                }

                if( nextLine.contains("[") || dataCheck ){

                    agentDataString += nextLine;

                    dataCheck = true;

                }

                if ( nextLine.contains("]")){


                    try {
                        Double radius = Double.parseDouble(agentDataString.substring(nextLine.indexOf("r=") + 4, agentDataString.indexOf(",")));
                        agentMap.get(agentIndex).setRadius(radius);
                    }
                    catch(NumberFormatException ex){
                        readingError = true;
                        break;
                    }


                    agentDataString = "";

                    dataCheck = false;

                }


                if( nextLine.contains("Start") ){
                    startCheck = true;
                    continue;
                }

            }

            if( agentCheck && startCheck && !goalCheck){

                if( nextLine.contains("->") ){


                    try {
                        agentIndex = Integer.parseInt(nextLine.substring(0, nextLine.indexOf("->")));
                        graphIndex = Integer.parseInt(nextLine.substring(nextLine.indexOf("->") + 2));
                    }
                    catch(NumberFormatException ex){
                        readingError = true;
                        break;
                    }

                    agentMap.get(agentIndex).setStartPosition(graphIndex);

                }

                if( nextLine.contains("Goal") ){
                    goalCheck = true;
                    continue;
                }

            }

            if( agentCheck && startCheck && goalCheck ){

                if( nextLine.contains("->") ){


                    try {
                        agentIndex = Integer.parseInt(nextLine.substring(0, nextLine.indexOf("->")));
                        graphIndex = Integer.parseInt(nextLine.substring(nextLine.indexOf("->") + 2));
                    }
                    catch(NumberFormatException ex){
                        readingError = true;
                        break;
                    }

                    agentMap.get(agentIndex).setTargetPosition(graphIndex);

                }

            }


        }


        if( readingError || !agentCheck || !startCheck || !goalCheck)
            agentMap.clear();


        return agentMap;

    }

    /**
     * Reads data from input file and creates a collection of Plans
     * @param path input file address and name
     * @return TreeMap<Integer, Plan> where an Integer value is a key identifier and a value is a Plan object  with this identifier
     **/
    public TreeMap<Integer, Plan> readPlan(String path) throws FileNotFoundException{

        File    file  = new File(path);
        Scanner input = new Scanner(file);

        boolean readingError = false;


        boolean planCheck = false;

        TreeMap<Integer, Plan> stepMap = new TreeMap<>();

        int agentIndex = 0;

        while(input.hasNextLine()){

            String nextLine = input.nextLine();

            if(nextLine.contains("{")){


                try {
                    agentIndex = Integer.parseInt(nextLine.substring(nextLine.indexOf("[") + 1, nextLine.indexOf("]")).trim());
                }
                catch(NumberFormatException ex){
                    readingError = true;
                    break;
                }


                stepMap.put(agentIndex, new Plan());

                planCheck = true;
                continue;
            }




            if( planCheck ){

                if(nextLine.contains("-->")){

                    try {

                        int fromIndex = Integer.parseInt(nextLine.substring(0, nextLine.indexOf("-->")).trim());
                        int toIndex = Integer.parseInt(nextLine.substring(nextLine.indexOf("-->") + 4, nextLine.indexOf("[")).trim());

                        double startTime = Double.parseDouble(nextLine.substring(nextLine.indexOf("[") + 1, nextLine.indexOf(",")).trim());
                        double finishTime = Double.parseDouble(nextLine.substring(nextLine.indexOf(",") + 1, nextLine.indexOf("]")).trim());

                        stepMap.get(agentIndex).addStep(new TimeInterval(startTime, finishTime), fromIndex, toIndex);

                    }
                    catch(NumberFormatException ex){
                        readingError = true;
                        break;
                    }



                }

            }

        }

        if(readingError)
            stepMap.clear();

        return stepMap;

    }


    private int getLocationAmount(String locationString){
        return Integer.parseInt(locationString);
    }

    private int getVertexAmount(String vertexString){
        return Integer.parseInt(vertexString);
    }

    private int getEdgeAmount(String edgeString){
        return Integer.parseInt(edgeString);
    }

}