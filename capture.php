<?php
// Start the session to capture cookies
session_start();

// Specify the file where cookies will be logged
$logFile = 'cookie.txt';

// Open the file for appending
$handle = fopen($logFile, 'a');

// Check if there are any cookies set
if (!empty($_COOKIE)) {
    // Loop through each cookie and log its name and value
    foreach ($_COOKIE as $name => $value) {
        // Format the cookie string
        $cookieLog = date('Y-m-d H:i:s') . " - $name: $value\n";
        
        // Write the cookie to the log file
        fwrite($handle, $cookieLog);
    }
    echo "<h1>Cookies have been logged.</h1>";
} else {
    echo "<h1>No cookies found.</h1>";
}

// Close the file handle
fclose($handle);

// Optionally, you can redirect or provide a link to go back
?>
<a href="javascript:history.back()">Go Back</a>
