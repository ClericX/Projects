<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <title>Filesystem Browser</title>
        <script type="text/javascript" src="syntaxhighlighter_3.0.83/scripts/shCore.js"></script>
        <script type="text/javascript" src="syntaxhighlighter_3.0.83/scripts/shBrushJScript.js"></script>
        <script type="text/javascript" src="syntaxhighlighter_3.0.83/scripts/shBrushCpp.js"></script>
        <script type="text/javascript" src="syntaxhighlighter_3.0.83/scripts/shBrushPlain.js"></script>
        <script type="text/javascript" src="syntaxhighlighter_3.0.83/scripts/shBrushPhp.js"></script>
        <script type="text/javascript" src="syntaxhighlighter_3.0.83/scripts/shBrushVb.js"></script>
        <script type="text/javascript" src="syntaxhighlighter_3.0.83/scripts/shBrushPython.js"></script>
        <script type="text/javascript" src="syntaxhighlighter_3.0.83/scripts/shBrushJava.js"></script>
        <script type="text/javascript" src="syntaxhighlighter_3.0.83/scripts/shBrushCss.js"></script>
        <script type="text/javascript" src="syntaxhighlighter_3.0.83/scripts/shBrushCSharp.js"></script>
        <script type="text/javascript" src="syntaxhighlighter_3.0.83/scripts/shBrushXml.js"></script>
        <link type="text/css" rel="stylesheet" href="syntaxhighlighter_3.0.83/styles/shCoreDefault.css"/>
        <script type="text/javascript">SyntaxHighlighter.all();</script>
        <style>
        /*
        body
        {
            background-color: #000000;
            padding: 0 0;
            margin: 0 0;
            background-image:url('Logo.png');
            background-repeat:no-repeat;
            background-attachment:fixed;
            background-position:center;
            color: white;
        }
        */

        a:link, a:visited
        {
            color: green;
        }

        table, td
        {
            border: 1px solid #000000;
        }

        td:hover
        {
            background-color: #CCCCCC;
        }
        </style>
    </head>

    <body>

<?php

function startsWith($haystack, $needle)
{
    return !strncmp($haystack, $needle, strlen($needle));
}

function endsWith($haystack, $needle)
{
    return substr($haystack, -strlen($needle)) === $needle;
}

function is_image($path)
{
    $a = getimagesize($path);
    $image_type = $a[2];
    
    if(in_array($image_type , array(IMAGETYPE_GIF , IMAGETYPE_JPEG ,IMAGETYPE_PNG , IMAGETYPE_BMP)))
    {
        return true;
    }
    return false;
}

function find_parent_dir($path)
{
    if (strlen($path) > 1)
    {
        for ($i = strlen($path) - 2; $i > 0; $i--)
        {
            if ($path[$i] == "/")
            {
                return substr($path, 0, $i + 1);
            }
        }
    }
    return "/";
}

$lowestPhysicalDir = str_replace("\\", "/", dirname(__FILE__)); // This can be just "/" for maximum script scope all the way from the root dir of the filesystem.
$loc = isset($_GET['loc']) ? $_GET['loc'] : "/";
$physicalLoc = endsWith($lowestPhysicalDir, "/") ? $lowestPhysicalDir . substr($loc, 1) : $lowestPhysicalDir . $loc;
// Might have to do some physical path > hyperlink parsing. This could get confusing if the lowest physical dir isn't the same as the script path.
// dirname(substr($_SERVER["REQUEST_URI"], 0, -strlen($_SERVER["QUERY_STRING"])))

if (is_dir($physicalLoc))
{
    // If for whatever reason the path doesn't end with a /, add one.
    // This shouldn't even happen unless the user tries to manually modify the loc value.
    if (!endsWith($loc, "/"))
    {
        $loc .= "/";
        $physicalLoc .= "/";
    }

    if ($hDir = opendir($physicalLoc))
    {
        $dirList = array();
        $fileList = array();

        // Iterate through the directory's contents, grouping directories and files separately.
        while (false !== ($entry = readdir($hDir)))
        {
            if ($entry != "." && $entry != "..")
            {
                if (is_dir($physicalLoc . $entry))
                {
                    $dirList[] = $entry;
                }
                else
                {
                    $fileList[] = $entry;
                }
            }
        }

        // Bubble sort directories and files alphabetically.
        // Sort directories first.
        $swapped = true;
        do {
            $swapped = false;
            for ($i=0; $i < count($dirList)-1; $i++)
            {
                if ($dirList[$i] > $dirList[$i+1])
                {
                    $temp = $dirList[$i];
                    $dirList[$i] = $dirList[$i+1];
                    $dirList[$i+1] = $temp;
                    $swapped = true;
                }
            }
        } while ($swapped);

        // Sort files after.
        $swapped = true;
        do {
            $swapped = false;
            for ($i=0; $i < count($fileList)-1; $i++)
            {
                if ($fileList[$i] > $fileList[$i+1])
                {
                    $temp = $fileList[$i];
                    $fileList[$i] = $fileList[$i+1];
                    $fileList[$i+1] = $temp;
                    $swapped = true;
                }
            }
        } while ($swapped);

        // Display contents nicely.
        echo "<table width='500px' border='1px' align='center'>";
        echo "<tr><td>Dir: " . $loc . "</td></tr>";

        if ($loc != "/")
        {
            echo "<tr><td><a href='?loc=" . find_parent_dir($loc) . "'>&lt;&lt; Previous Directory</a></td></tr>";
        }

        // Print the directory list.
        for ($i = 0; $i < count($dirList); $i++)
        {
            echo "<tr><td><a href='?loc=" . ($loc . $dirList[$i]) . "/'>/" . $dirList[$i] . "/</a></td></tr>";
        }

        // Print the file list.
        for ($i = 0; $i < count($fileList); $i++)
        {
            echo "<tr><td><a href='?loc=" . ($loc . $fileList[$i]) . "'>" . $fileList[$i] . "</a></td></tr>";
        }

        echo "</table>";

        closedir($hDir);
    }
    else
    {
        echo "Access to the desired directory was denied.";
    }
}
else if (is_file($physicalLoc))
{
    echo "<a href='?loc=" . find_parent_dir($loc) . "'>Back</a><br />";
    echo "<div style='border: 1px solid #000000'>";

    $locFilesize = filesize($physicalLoc);

    if ($locFilesize > 0)
    {
        if (is_image($physicalLoc))
        {
            // TODO: Acquire image contents through a script for outputting because this way won't work for images without an accessable relative path.
            echo "<img src='" . substr($loc, 1) . "'></img>";
        }
        else
        {
            $hFile = fopen($physicalLoc, "rb");
            if ($hFile)
            {
                echo "<pre class='brush: ";
                $fileExt = substr($physicalLoc, strrpos($physicalLoc, ".") + 1);
                // Prevent other filetypes from just breaking the formatter.
                if (in_array($fileExt, array("js", "cpp", "c", "txt", "php", "vb", "py", "java", "css", "cs", "xml", "html", "htm", "xml", "h", "hpp")))
                {
                    echo $fileExt;
                }
                else
                {
                    echo "txt";
                }
                echo ";'>";
                $contents = fread($hFile, $locFilesize);
                fclose($hFile);
                echo htmlspecialchars($contents);
                echo "</pre>";
            }
            else
            {
                echo "The file could not be opened for reading.";
            }
        }
    }
    else
    {
        echo "File is empty.";
    }

    echo "</div>";
}
else
{
    echo "Invalid location specified.";
}

?>

    </body>

</html>