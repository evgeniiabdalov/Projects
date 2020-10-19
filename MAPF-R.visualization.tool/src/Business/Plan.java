package Business;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author abdalevg
 */
public class Plan {

    private List<Step> stepList;

    public Plan() {
        stepList = new ArrayList<>();
    }

   public void addStep(TimeInterval timeInterval, int from, int to){
        this.stepList.add(new Step(timeInterval, from, to));
    }

    public List<Step> getStepList(){
        return this.stepList;
    }

    @Override
    public String toString(){

        String scheduleString = "{\n";

        for(Step stepItem : this.stepList){
            scheduleString += stepItem.toString();
        }

        scheduleString += "}\n";

        return scheduleString;

    }




}