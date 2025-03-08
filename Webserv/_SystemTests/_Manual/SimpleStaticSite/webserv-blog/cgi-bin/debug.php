<?php
header("Content-Type: text/plain");

echo "Request Method: " . $_SERVER['REQUEST_METHOD'] . "\n";
echo "Query String: " . $_SERVER['QUERY_STRING'] . "\n";

// Read raw POST body
$body = file_get_contents("php://input");
echo "Raw Body:\n" . $body . "\n";

// Show multipart/form-data if it exists
if (!empty($_FILES)) {
    echo "Uploaded Files:\n";
    print_r($_FILES);
}
if (!empty($_POST)) {
    echo "POST Data:\n";
    print_r($_POST);
}

// Show HTTP Headers
echo "Headers:\n";
foreach (getallheaders() as $name => $value) {
    echo "$name: $value\n";
}
?>