package Visualization;

import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;

public class AgentVisual {

      private Circle agentCircle;

      public        AgentVisual( Integer ID, Double radius){

           agentCircle = new Circle();

           agentCircle.setId(ID.toString());
           agentCircle.setRadius(radius*100);
           agentCircle.setStrokeWidth(20);
           agentCircle.setVisible(true);

           System.out.println(agentCircle);
           System.out.println("------------------------------------------");

      }

      public Circle getAgentCircle() {
          return agentCircle;
      }


}
