package GUI;

import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;

import java.util.ArrayList;
import java.util.List;

public class GraphComponents {

   private List<Line>   graphLines = new ArrayList<>();
   private List<Circle> graphCircles = new ArrayList<>();

   private boolean isVisible;

   public GraphComponents(){
      isVisible = true;
   }

   public void addLine(Line edge){
      graphLines.add(edge);
   }

   public void addCircle(Circle vertex){
      graphCircles.add(vertex);
   }

   public void changeVisibility(){
      if(isVisible) {
         makeInvisible();
         isVisible = false;
      }
      else {
         makeVisible();
         isVisible = true;
      }
   }

   private void makeVisible(){

      for( Line edge : this.graphLines )
         edge.setVisible(true);

      for( Circle vertex : this.graphCircles )
         vertex.setVisible(true);

   }

   private void makeInvisible(){

      for( Line edge : this.graphLines )
         edge.setVisible(false);

      for( Circle vertex : this.graphCircles )
         vertex.setVisible(false);

   }



}
