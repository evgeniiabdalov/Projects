package Business;

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
