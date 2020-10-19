package Statistics;

public class AgentTimeRatio {

    private Integer  movingTime;
    private Integer  waitingTime;
    private Integer  initializingTime;
    private Integer  arrivedTime;


    public AgentTimeRatio(){
        movingTime = 0;
        waitingTime = 0;
        initializingTime = 0;
        arrivedTime = 0;
    }

    public Integer getMovingTime() {
        return movingTime;
    }

    public void setMovingTime(Integer movingTime) {
        System.out.println("setting moving time " + movingTime);
        this.movingTime = movingTime;
    }

    public Integer getWaitingTime() {
        return waitingTime;
    }

    public void setWaitingTime(Integer waitingTime) {
        System.out.println("setting waiting time " + waitingTime);
        this.waitingTime = waitingTime;
    }

    public Integer getInitializingTime() {
        return initializingTime;
    }

    public void setInitializingTime(Integer initializingTime) {
        System.out.println("setting initializing time " + initializingTime);
        this.initializingTime = initializingTime;
    }

    public Integer getArrivedTime() {
        return arrivedTime;
    }

    public void setArrivedTime(Integer arrivedTime) {
        System.out.println("setting arrived time " + arrivedTime);
        this.arrivedTime = arrivedTime;
    }


}
