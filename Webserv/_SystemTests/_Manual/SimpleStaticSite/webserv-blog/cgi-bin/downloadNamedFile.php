#!/usr/bin/php
<?php

// Fetch QUERY_STRING and parse it manually
$query_string = getenv("QUERY_STRING");
parse_str($query_string, $query);


// Check if 'name' is set and valid
$filename = "random_file.txt"; // Default name
if (isset($query['name']) && preg_match('/^[a-zA-Z0-9._-]+$/', $query['name'])) {
    $filename = $query['name'];
}

//valid headers
header("Content-Type: application/octet-stream");
header("Content-Disposition: attachment; filename=$filename");

// Generate random content
$size = rand(1024, 4000000); // Random file size between 1KB and 4KB
$data = '';

for ($i = 0; $i < $size; $i++) {
    $data .= chr(rand(32, 126)); 
}

// Send the file content
echo $data;
?>