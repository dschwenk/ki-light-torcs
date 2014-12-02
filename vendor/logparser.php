<?php

$handle = fopen("logfile.txt", "r");
$new = fopen("parsedLogfile.txt", 'w');
if ($handle) {
    while (($line = fgets($handle)) !== false) {

	$line = str_replace(array("\r", "\n", "\r\n"), "", $line);
echo "start";

	if (strpos($line, 'angle') !== false) {
	    $angle = trim(explode(')', explode('angle', $line)[1])[0]);
	    fwrite($new, $angle . ',');
	    echo "<br>angle" . $angle;
	}

	if (strpos($line, 'distFromStart') !== false) {
	    $distFromStart = trim(explode(')', explode('distFromStart', $line)[1])[0]);
	    fwrite($new, $distFromStart . ',');
	    echo "<br>distFromStart" . $distFromStart;
	}

	if (strpos($line, 'gear') !== false) {
	    $gear = trim(explode(')', explode('gear', $line)[1])[0]);
	    fwrite($new, $gear . ',');
	    echo "<br>gear" . $gear;
	}
        
	if (strpos($line, 'lastLapTime') !== false) {
	    $lastLapTime = trim(explode(')', explode('lastLapTime', $line)[1])[0]);
	    fwrite($new, $lastLapTime . ',');
	    echo "<br>lastLapTime" . $lastLapTime;
	}

	if (strpos($line, 'opponents') !== false) {
	    $opponents = trim(explode(')', explode('opponents', $line)[1])[0]);
	    fwrite($new, str_replace(' ', ',', $opponents) . ',');
	    echo "<br>opponents" . str_replace(' ', ',', $opponents);
	}

	if (strpos($line, 'rpm') !== false) {
	    $rpm = trim(explode(')', explode('rpm', $line)[1])[0]);
	    fwrite($new, $rpm . ',');
	    echo "<br>rpm" . $rpm;
	}

	if (strpos($line, 'speedX') !== false) {
	    $speedX = trim(explode(')', explode('speedX', $line)[1])[0]);
	    fwrite($new, $speedX . ',');
	    echo "<br>speedX" . $speedX;
	}

	if (strpos($line, 'speedY') !== false) {
	    $speedY = trim(explode(')', explode('speedY', $line)[1])[0]);
	    fwrite($new, $speedY . ',');
	    echo "<br>speedY" . $speedY;
	}

	if (strpos($line, 'speedZ') !== false) {
	    $speedZ = trim(explode(')', explode('speedZ', $line)[1])[0]);
	    fwrite($new, $speedZ . ',');
	    echo "<br>speedZ" . $speedZ;
	}

	if (strpos($line, 'track') !== false) {
	    $track = trim(explode(')', explode('track', $line)[1])[0]);
	    fwrite($new, str_replace(' ', ',', $track) . ',');
	    echo "<br>track" . str_replace(' ', ',', $track);
	}

	if (strpos($line, 'trackPos') !== false) {
	    $trackPos = trim(explode(')', explode('trackPos', $line)[1])[0]);
	    fwrite($new, $trackPos . ',');
	    echo "<br>trackPos" . $trackPos;
	}

	if (strpos($line, 'accel') !== false) {
	    $accel = trim(explode(')', explode('accel', $line)[1])[0]);
	    fwrite($new, $accel . ',');
	    echo "<br>accel" . $accel;
	}

	if (strpos($line, 'brake') !== false) {
	    $brake = trim(explode(')', explode('brake', $line)[1])[0]);
	    fwrite($new, $brake . ',');
	    echo "<br>brake" . $brake;
	}

	if (strpos($line, 'steer') !== false) {
	    $steer = trim(explode(')', explode('steer', $line)[1])[0]);
	    fwrite($new, $steer);
	    echo "<br>steer" . $steer;
	}
	
	if (strpos($line, 'accel') !== false) {
	    fwrite($new, "\n");
	}
    }
} else {
    // error opening the file.
} 
fclose($handle);
fclose($new);
