BEGIN	{ print "<html>\n";
	  print "<body>\n";
	  print "<h1>Man Pages for Tipsy Commands</h1>\n";
	}
	{ $1 = "<a href="$1".html>"$1"</a>" ;
	print $0, "<br>" ;
	}
END	{ print "</body>\n" ;
          print "</html>\n" ;
	}
