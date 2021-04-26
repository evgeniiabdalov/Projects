package Statistics;

import javafx.util.Pair;

import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;

/**
 * CollisionData keeps data about collision events between Agents
 * @author Evgenii Abdalov
 */
public class CollisionData {

    /**
     * TreeMap<Integer, Pair<Integer, Integer>> collisionTimeMap, where key is an index of collisionTimeList,
     * and value is Pair<Integer, Integer>, where Integer values are unique identifiers
     * of two Agents involved in collisiion event
     */
    private TreeMap<Integer, Pair<Integer, Integer>> collisionTimeMap;

    /**
     * List<Pair<Double, Double>> collisionTimeList, where value is Pair<Double, Double>,
     * where Double values are moment in time, start of collision event and finish of collision event
     */
    private List<Pair<Double, Double>> collisionTimeList;


    public CollisionData(){

        this.collisionTimeMap  = new TreeMap<>();
        this.collisionTimeList = new ArrayList<>();

    }

    public TreeMap<Integer, Pair<Integer, Integer>> getCollisionTimeMap() {
        return collisionTimeMap;
    }

    public void setCollisionTimeMap(TreeMap<Integer, Pair<Integer, Integer>> collisionTimeMap) {
        this.collisionTimeMap = collisionTimeMap;
    }

    public List<Pair<Double, Double>> getCollisionTimeList() {
        return collisionTimeList;
    }

    public void setCollisionTimeList(List<Pair<Double, Double>> collisionTimeList) {
        this.collisionTimeList = collisionTimeList;
    }
    public void printCollisionData(){
        System.out.println("COLLISION DATA");

        for(int i = 0; i < this.collisionTimeList.size(); i++){
            System.out.println("time pair " + this.collisionTimeList.get(i));
            System.out.println("agent pair " + this.collisionTimeMap.get(i));
        }
    }

}
