<?php
   $estado = -1;
   if (isset($_POST["estado"]) && !empty($_POST["estado"]))
      $estado = $_POST["estado"];
   if ($estado > -1)
   {
      // Tener en cuenta los permisos del ttyACM0 y que el puerto sea ese exactamente
      // chmod 662 ttyACM0 <---

      $f = fopen ("/dev/ttyACM0","w");

      // SLEEP para evitar que se pierda la escritura en el puerto serie debido al RESET que se produce con cada fopen
      // Se puede evitar el RESET añadiendo al montaje un condensador de 10uF entre RESET(+) y GND (-)
   
      // sleep(2); 

      fwrite($f,$estado);
      fclose($f);
   }
?>
<!doctype html>
<html>
<head></head>
<body>
<form method=post action=arduwww.php>
   <img src="<?php if ($estado==1) echo 'on'; else echo 'off' ?>.png"><br>
   <input type="radio" name="estado" value="1" <?php if ($estado==1) echo "checked" ?>> Encender<br/>
   <input type="radio" name="estado" value="2" <?php if ($estado==2) echo "checked" ?>> Apagar<br/> 
   <br /> 
   <input type="submit" value="Enviar" style="width: 15em">
</form>
</body>
</html>
