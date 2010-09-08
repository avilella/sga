#! /usr/bin/perl

use strict;
use Getopt::Long;

my $attrFile = "";

# Params
my $bDirected = 0;
my $bUseShapes = 1;
my $bUseLabels = 0;
my $defaultShape = "point";
GetOptions("attribute=s" => \$attrFile);

my %vertexColors;
my %vertexShapes;
my %vertexSizes;
if($attrFile ne "")
{
    loadVertexAttributes($attrFile);
}

my $graphAttr;
if($bDirected)
{
    $graphAttr = "digraph";
}
else
{
    $graphAttr = "graph";
}

print "$graphAttr G {\n";

while(<>)
{
	if(/VT/)
	{
		my @record = split;
        my $id = $record[1];
        my @attributes;
        push @attributes, makeAttribute("label", quoteStr($id)) if $bUseLabels;
        push @attributes, makeAttribute("shape", getVertexShape($id)) if $bUseShapes;
        push @attributes, makeAttribute("fillcolor", getVertexColor($id));
        push @attributes, makeAttribute("style", "filled");
        push @attributes, makeAttribute("width", getVertexSize($id));
        push @attributes, makeAttribute("fixedsize", 1);
        my $attrStr = attributes2string(@attributes);
		print quoteStr($id) . " " . $attrStr . ";\n" 	
	}

	if(/ED/)
	{
		my @record = split;
		my $ol = $record[4] - $record[3] + 1;
        my $s1 = $record[3];
        my $s2 = $record[6];
		print quoteStr($record[1]) . " " . getArrow() . " " . quoteStr($record[2]) . " " . makeLabel($ol, getEdgeColor($s1)) . ";\n";
        if($bDirected)
        {
            print quoteStr($record[2]) . " " . getArrow() . " " . quoteStr($record[1]) . " " . makeLabel($ol, getEdgeColor($s2)) . ";\n";
        }
	}
}

print "}\n";

sub quoteStr
{
	my ($s) = @_;
	return qq(") . $s . qq(");
}

sub makeLabel
{
	my($l, $color) = @_;
    my $c_str;
    if($color ne "")
    {
        $c_str = "color=\"$color\"";
    }
	my $str = qq([label=") . $l . qq(" ) . $c_str. qq(]);
	return $str;
}

sub makeAttribute
{
    my($name, $value) = @_;
    return $name . "=" . $value;
}

sub attributes2string
{
    return "[" . join(" ", @_) . "]";
}

sub getArrow
{
    if($bDirected)
    {
        return "->";
    }
    else
    {
        return "--"
    }
}

sub getEdgeColor
{
    my($s) = @_;
    if($s == 0)
    {
        return "red";
    }
    else
    {
        return "black";
    }
}

sub loadVertexAttributes
{
    my($file) = @_;
    open(F, $file);
    while(<F>)
    {
        chomp;
        next if /ID/; #skip header
        next if $_ eq ""; #skip blank

        my @fields = split;
        my $id = $fields[0];
        my $len = $fields[1];
        my $f1 = $fields[5];
        my $f2 = $fields[6];
        
        my $color = "gray";
        if($f1 > 0.75)
        {
            $color = "red";
        }
        elsif($f2 > 0.75)
        {
            $color = "blue";
        }
        else
        {
            $color = "gray";
        }

        $vertexColors{$id} = $color;

        # Compute the shape of the vertex
        $vertexShapes{$id} = $defaultShape;

        # Compute vertex size
        if($len > 1000)
        {
            $vertexSizes{$id} = 1.0;
        }
        if($len > 500)
        {
            $vertexSizes{$id} = 0.5;
        }
        else
        {
            $vertexSizes{$id} = 0.2;
        }
    }
}

sub getVertexColor
{
    my($id) = @_;
    return "white" if !defined($vertexColors{$id});
    return $vertexColors{$id};
}

sub getVertexShape
{
    my($id) = @_;
    return "circle" if !$bUseShapes || !defined($vertexShapes{$id});
    return $vertexShapes{$id};
}

sub getVertexSize
{
    my($id) = @_;
    return 1 if !defined($vertexSizes{$id});
    return $vertexSizes{$id};
}
