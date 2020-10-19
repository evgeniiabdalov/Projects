package Statistics;

import javafx.util.Pair;

import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;

public class CollisionData {

    private TreeMap<Integer, Pair<Integer, Integer>> collisionTimeMap;
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
