
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
 *
 * @author abdalevg
 */
public class DataReader {

    public Graph readGraph(String path) throws FileNotFoundException{

        File    file  = new File(path);
        Scanner input = new Scanner(file);
        Graph   map   = new Graph();

        int     vertexIndex = 0;
        double  xCoordinate = 0;
        double  yCoordinate = 0;
        int     fromIndex   = 0;
        int     toIndex     = 0;


        int locationAmount = 0;
        int vertexAmount   = 0;
        int edgeAmount     = 0;

        boolean locationAmountCheck = false;
        boolean vertexAmountCheck   = false;
        boolean edgeAmountCheck     = false;

        boolean locationSubstringCheck = true;
        boolean vertexSubstringCheck   = true;
        boolean edgeSubstringCheck     = true;



        while(input.hasNext()){

            String nextLine =  input.next();


            if( locationSubstringCheck ){
                if( nextLine.contains("Locations:") ){
                    locationAmountCheck = true;
                    locationSubstringCheck = false;
                    continue;
                }
            }
            if(  vertexSubstringCheck ){
                if( nextLine.contains("Vertices:") ){
                    vertexAmountCheck = true;
                    vertexSubstringCheck = false;
                    continue;
                }
            }
            if( edgeSubstringCheck  ){
                if( nextLine.contains("Edges:") ){
                    edgeAmountCheck = true;
                    edgeSubstringCheck  = false;
                    continue;
                }
            }



            if( locationAmountCheck ){
                locationAmount = this.getLocationAmount(nextLine);
                locationAmountCheck = false;
                continue;
            }
            if( vertexAmountCheck ){
                vertexAmount = this.getVertexAmount(nextLine);
                vertexAmountCheck = false;
                continue;
            }
            if( edgeAmountCheck ){
                edgeAmount = this.getEdgeAmount(nextLine);
                edgeAmountCheck = false;
                continue;
            }


            if( !locationSubstringCheck && vertexSubstringCheck && edgeSubstringCheck ){

                if( nextLine.contains(":") ){
                    vertexIndex = Integer.parseInt(nextLine.substring(0, nextLine.indexOf(":")));
                }
                else if( nextLine.contains(",") ){
                    xCoordinate = Double.parseDouble(nextLine.substring(0, nextLine.indexOf(",")));
                }
                else{
                    yCoordinate = Double.parseDouble(nextLine);

                    map.addVertex( vertexIndex, xCoordinate, yCoordinate);
                }

            }

            if( !locationSubstringCheck && !vertexSubstringCheck && !edgeSubstringCheck ){
                fromIndex = Integer.parseInt(nextLine.substring(nextLine.indexOf("{")+1, nextLine.indexOf(",")));
                toIndex   = Integer.parseInt(nextLine.substring(nextLine.indexOf(",")+1, nextLine.indexOf("}")));

                map.addEdge(fromIndex, toIndex);
            }


        }


        return map;

    }

    public TreeMap<Integer, Agent> readAgents(String path) throws FileNotFoundException{

        File    file  = new File(path);
        Scanner input = new Scanner(file);

        TreeMap<Integer, Agent> agentMap = new TreeMap<>();



        boolean agentCheck     = false;
        boolean startCheck    = false;
        boolean goalCheck     = false;
        boolean dataCheck     = false;


        int     agentIndex    = 0;
        int     graphIndex    = 0;

        String  agentDataString = "";

        while(input.hasNext()){

            String nextLine = input.next();


            if( !agentCheck && !startCheck && !goalCheck ){

                if( nextLine.contains("Kruhobots") ){
                    agentCheck = true;
                    continue;
                }

            }


            if( agentCheck && !startCheck && !goalCheck ){

                if( nextLine.contains(":") ){
                    agentIndex= Integer.parseInt(nextLine.substring(0, nextLine.indexOf(":")));
                    agentMap.put(agentIndex, new Agent(agentIndex));
                }

                if( nextLine.contains("[") || dataCheck ){

                    agentDataString += nextLine;

                    dataCheck = true;

                }

                if ( nextLine.contains("]")){


                    Double radius = Double.parseDouble(agentDataString.substring(nextLine.indexOf("r=") + 4, agentDataString.indexOf(",")));
                    agentMap.get(agentIndex).setRadius(radius);


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

                    agentIndex = Integer.parseInt(nextLine.substring(0, nextLine.indexOf("->")));
                    graphIndex = Integer.parseInt(nextLine.substring(nextLine.indexOf("->") + 2));

                    agentMap.get(agentIndex).setStartPosition(graphIndex);

                }

                if( nextLine.contains("Goal") ){
                    goalCheck = true;
                    continue;
                }

            }

            if( agentCheck && startCheck && goalCheck ){

                if( nextLine.contains("->") ){

                    agentIndex = Integer.parseInt(nextLine.substring(0, nextLine.indexOf("->")));
                    graphIndex = Integer.parseInt(nextLine.substring(nextLine.indexOf("->") + 2));

                    agentMap.get(agentIndex).setTargetPosition(graphIndex);

                }

            }


        }

        return agentMap;

    }

    public TreeMap<Integer, Plan> readPlan(String path) throws FileNotFoundException{

        File    file  = new File(path);
        Scanner input = new Scanner(file);

        boolean planCheck = false;

        TreeMap<Integer, Plan> stepMap = new TreeMap<>();

        int agentIndex = 0;

        while(input.hasNextLine()){

            String nextLine = input.nextLine();



            if(nextLine.contains("{")){

                agentIndex = Integer.parseInt(nextLine.substring(nextLine.indexOf("[") + 1, nextLine.indexOf("]")).trim());


                stepMap.put(agentIndex, new Plan());

                planCheck = true;
                continue;
            }




            if( planCheck ){

                if(nextLine.contains("-->")){

                    int fromIndex = Integer.parseInt(nextLine.substring(0, nextLine.indexOf("-->")).trim());
                    int toIndex   = Integer.parseInt(nextLine.substring(nextLine.indexOf("-->") + 4, nextLine.indexOf("[")).trim());

                    double startTime  = Double.parseDouble(nextLine.substring(nextLine.indexOf("[") + 1, nextLine.indexOf(",")).trim());
                    double finishTime = Double.parseDouble(nextLine.substring(nextLine.indexOf(",") + 1, nextLine.indexOf("]")).trim());

                    stepMap.get(agentIndex).addStep(new TimeInterval(startTime, finishTime), fromIndex, toIndex);

                }

            }

        }

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