package Business;

/**
 * AgentState class is responsible for establishing a correlation between String
 * and Integer values of state. For instance, value 0 indicates that Agent is in
 * state Initializing, etc.
 * @author Evgenii Abdalov
 */
public class AgentState {

    public String getState(Integer stateIndex){

        if( stateIndex == 0 )
            return "INITIALIZING";
        if( stateIndex == 1 )
            return "MOVING";
        if(  stateIndex == 2 )
            return "WAITING";
        if( stateIndex == 3 )
            return "ARRIVED";


        return "INITIALIZING";

    }

    public Integer getStateIndex(String state){

        if( state.equals("INITIALIZING") )
            return 0;
        if( state.equals("MOVING") )
            return 1;
        if(  state.equals("WAITING") )
            return 2;
        if( state.equals("ARRIVED") )
            return 3;

        return 0;

    }



}
