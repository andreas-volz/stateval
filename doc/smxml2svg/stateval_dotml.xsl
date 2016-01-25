<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:key name="child-states" match="state" use="@parent" />
<xsl:template match="state" mode="make-node">
   <cluster id="{@name}" label="{@name}" fontname="Arial" fontsize="9">
     <xsl:apply-templates mode="make-node" select="key('child-states', @name)" />
   </cluster>
</xsl:template>

<xsl:template match="/">
    <dotml:graph file-name="stateval" xmlns:dotml="http://www.martin-loetzsch.de/DOTML"
      xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
      xsi:schemaLocation="http://www.martin-loetzsch.de/DotML ../dotml-1.3/dotml-1.3.xsd" label="stateval" fontcolor="#0000A0" fontname="Arial Bold" margin="0.2,0.1" ranksep="0.2" nodesep="0.5"
      bgcolor="#F0F0FF" fontsize="13" style="dashed">

        <xsl:apply-templates select="stateval/states" mode="make-node"/>
      
        <xsl:for-each select="stateval/transitions/transition">
            <dotml:edge>
                <xsl:attribute name="from"><xsl:value-of select="@from"/>
                </xsl:attribute>

                <xsl:attribute name="to"><xsl:value-of select="@to"/>
                </xsl:attribute>

                <xsl:attribute name="label"><xsl:value-of select="@event"/>
                </xsl:attribute>

                <xsl:attribute name="fontname">Arial</xsl:attribute>

                <xsl:attribute name="fontsize">7</xsl:attribute>
            </dotml:edge>
        </xsl:for-each>

    </dotml:graph>
</xsl:template>

</xsl:stylesheet>
