/*
 * Copyright (c) 2017, RISE SICS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

#include "contiki.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-sr.h"
#include "os/lib/json/json.h"
#include <stdio.h>
#include <string.h>
/*---------------------------------------------------------------------------*/
static const char *TOP = "<html>\n  <head>\n    <title>Contiki-NG</title>\n  </head>\n<body>\n";
static const char *BOTTOM = "\n</body>\n</html>\n";
static char buf[8000];
static int blen;
#define ADD(...) do {                                                   \
    blen += snprintf(&buf[blen], sizeof(buf) - blen, __VA_ARGS__);      \
  } while(0);
#define SEND(s) do { \
  SEND_STRING(s, buf); \
  blen = 0; \
} while(0);

/* Use simple webserver with only one page for minimum footprint.
 * Multiple connections can result in interleaved tcp segments since
 * a single static buffer is used for all segments.
 */
#include "httpd-simple.h"
static char string[] = "\
		<!DOCTYPE html>\n\
		<!-- saved from url=(0038)http://bl.ocks.org/d3noob/raw/8375092/ -->\n\
		<html lang=\"en\"><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n\
		    <title>Tree Example</title>\n\
		    <style>\n\
			.node {\n\
				cursor: pointer;\n\
			}\n\
			.node circle {\n\
			  fill: #fff;\
			  stroke: steelblue;\n\
			  stroke-width: 3px;\n\
			}\n\
			.node text {\n\
			  font: 12px sans-serif;\n\
			}\n\
			.link {\n\
			  fill: none;\n\
			  stroke: #ccc;\n\
			  stroke-width: 2px;\n\
			}\n\
		    </style>\n\
		  </head>\n\
		  <body>\n\
		<!-- load the d3.js library -->	\n\
		<script src=\"./test1_files/d3.v3.min.js\"></script>	\n\
		<script>\n\
		";
static char treedata[]="var treeData = \n\
		  [\n\
		  {\n\
		    \"name\": \"fd00:212:7401:1:101\",\n\
		    \"parent\": \"null\",\n\
		    \"children\": [\n\
		      {\n\
		        \"name\": \"fd00:212:7403:3:103\",\n\
		        \"parent\": \"fd00:212:7401:1:101\",\n\
		        \"children\": [\n\
		          {\n\
		            \"name\": \"fd00:212:7403:3:103\",\n\
		            \"parent\": \"fd00:212:7401:1:101\"\n\
		          },\n\
		          {\n\
		            \"name\": \"fd00:212:740b:b:10b\",\n\
		            \"parent\": \"fd00:212:7403:3:103\"\n\
		          }\n\
		        ]\n\
		      },\n\
		      {\n\
		        \"name\": \"fd00:212:7407:7:107\",\n\
		        \"parent\": \"fd00:212:7401:1:101\"\n\
		      }\n\
		    ]\n\
		  }\n\
		];\n";

static char string2[]="\
		var margin = {top: 20, right: 120, bottom: 20, left: 120},\n\
			width = 960 - margin.right - margin.left,\n\
			height = 500 - margin.top - margin.bottom;\n\
		var i = 0,\n\
			duration = 750,\n\
		var	root;\n\
		var tree = d3.layout.tree()\n\
			.size([height, width]);\n\
		var diagonal = d3.svg.diagonal()\n\
			.projection(function(d) { return [d.y, d.x]; });\n\
		var svg = d3.select(\"body\").append(\"svg\")\n\
			.attr(\"width\", width + margin.right + margin.left)\n\
			.attr(\"height\", height + margin.top + margin.bottom)\n\
		  .append(\"g\")\n\
			.attr(\"transform\", \"translate(\" + margin.left + \",\" + margin.top + \")\");\n\
		root = treeData[0];\n\
		root.x0 = height / 2;\n\
		root.y0 = 0;\n\
		update(root);\n\
		d3.select(self.frameElement).style(\"height\", \"500px\");\n\
		function update(source) {\n\
		  // Compute the new tree layout.\n\
		  var nodes = tree.nodes(root).reverse(),\n\
			  links = tree.links(nodes);\n\
		  // Normalize for fixed-depth.\n\
		  nodes.forEach(function(d) { d.y = d.depth * 180; });\n\
		  // Update the nodes¡­\n\
		  var node = svg.selectAll(\"g.node\")\n\
			  .data(nodes, function(d) { return d.id || (d.id = ++i); });\n\
		  // Enter any new nodes at the parent\'s previous position.\n\
		  var nodeEnter = node.enter().append(\"g\")\n\
			  .attr(\"class\", \"node\")\n\
			  .attr(\"transform\", function(d) { return \"translate(\" + source.y0 + \",\" + source.x0 + \")\"; })\n\
			  .on(\"click\", click);\n\
		  nodeEnter.append(\"circle\")\n\
			  .attr(\"r\", 1e-6)\n\
			  .style(\"fill\", function(d) { return d._children ? \"lightsteelblue\" : \"#fff\"; });\n\
		  nodeEnter.append(\"text\")\n\
			  .attr(\"x\", function(d) { return d.children || d._children ? -13 : 13; })\n\
			  .attr(\"dy\", \".35em\")\n\
			  .attr(\"text-anchor\", function(d) { return d.children || d._children ? \"end\" : \"start\"; })\n\
			  .text(function(d) { return d.name; })\n\
			  .style(\"fill-opacity\", 1e-6);\n\
		  // Transition nodes to their new position.\n\
		  var nodeUpdate = node.transition()\n\
			  .duration(duration)\n\
			  .attr(\"transform\", function(d) { return \"translate(\" + d.y + \",\" + d.x + \")\"; });\n\
		  nodeUpdate.select(\"circle\")\n\
			  .attr(\"r\", 10)\n\
			  .style(\"fill\", function(d) { return d._children ? \"lightsteelblue\" : \"#fff\"; });\n\
		  nodeUpdate.select(\"text\")\n\
			  .style(\"fill-opacity\", 1);\n\
		  // Transition exiting nodes to the parent\'s new position.\n\
		  var nodeExit = node.exit().transition()\n\
			  .duration(duration)\n\
			  .attr(\"transform\", function(d) { return \"translate(\" + source.y + \",\" + source.x + \")\"; })\n\
			  .remove();\n\
		  nodeExit.select(\"circle\")\n\
			  .attr(\"r\", 1e-6);\n\
		  nodeExit.select(\"text\")\n\
			  .style(\"fill-opacity\", 1e-6);\n\
		  // Update the links¡­\n\
		  var link = svg.selectAll(\"path.link\")\n\
			  .data(links, function(d) { return d.target.id; });\n\
		  // Enter any new links at the parent\'s previous position.\n\
		  link.enter().insert(\"path\", \"g\")\n\
			  .attr(\"class\", \"link\")\n\
			  .attr(\"d\", function(d) {\n\
				var o = {x: source.x0, y: source.y0};\n\
				return diagonal({source: o, target: o});\n\
			  });\n\
		  // Transition links to their new position.\n\
		  link.transition()\n\
			  .duration(duration)\n\
			  .attr(\"d\", diagonal);\n\
		  // Transition exiting nodes to the parent\'s new position.\n\
		  link.exit().transition()\n\
			  .duration(duration)\n\
			  .attr(\"d\", function(d) {\n\
				var o = {x: source.x, y: source.y};\n\
				return diagonal({source: o, target: o});\n\
			  })\n\
			  .remove();\n\
		  // Stash the old positions for transition.\n\
		  nodes.forEach(function(d) {\n\
			d.x0 = d.x;\n\
			d.y0 = d.y;\n\
		  });\n\
		}\n\
		// Toggle children on click.\n\
		function click(d) {\n\
		  if (d.children) {\n\
			d._children = d.children;\n\
			d.children = null;\n\
		  } else {\n\
			d.children = d._children;\n\
			d._children = null;\n\
		  }\n\
		  update(d);\n\
		}\n\
		</script>\n\
		<svg width=\"960\" height=\"500\"><g transform=\"translate(120,20)\"><path class=\"link\" d=\"M180,197.1428571428571C270,197.1428571428571 270,131.42857142857142 360,131.42857142857142\"></path><path class=\"link\" d=\"M180,197.1428571428571C270,197.1428571428571 270,262.85714285714283 360,262.85714285714283\"></path><path class=\"link\" d=\"M0,262.85714285714283C90,262.85714285714283 90,197.1428571428571 180,197.1428571428571\"></path><path class=\"link\" d=\"M0,262.85714285714283C90,262.85714285714283 90,328.57142857142856 180,328.57142857142856\"></path><g class=\"node\" transform=\"translate(180,328.5714416503906)\"><circle r=\"10\" style=\"fill: rgb(255, 255, 255);\"></circle><text x=\"13\" dy=\".35em\" text-anchor=\"start\" style=\"fill-opacity: 1;\">Level 2: B</text></g><g class=\"node\" transform=\"translate(360,262.8571472167969)\"><circle r=\"10\" style=\"fill: rgb(255, 255, 255);\"></circle><text x=\"13\" dy=\".35em\" text-anchor=\"start\" style=\"fill-opacity: 1;\">Daughter of A</text></g><g class=\"node\" transform=\"translate(360,131.42857360839844)\"><circle r=\"10\" style=\"fill: rgb(255, 255, 255);\"></circle><text x=\"13\" dy=\".35em\" text-anchor=\"start\" style=\"fill-opacity: 1;\">Son of A</text></g><g class=\"node\" transform=\"translate(180,197.14285278320312)\"><circle r=\"10\" style=\"fill: rgb(255, 255, 255);\"></circle><text x=\"-13\" dy=\".35em\" text-anchor=\"end\" style=\"fill-opacity: 1;\">Level 2: A</text></g><g class=\"node\" transform=\"translate(0,262.8571472167969)\"><circle r=\"10\" style=\"fill: rgb(255, 255, 255);\"></circle><text x=\"-13\" dy=\".35em\" text-anchor=\"end\" style=\"fill-opacity: 1;\">fd00:212:7401:1:101</text></g></g></svg>\n\
			\n\
		  \n\
		</body></html>\n\
		";
/*---------------------------------------------------------------------------*/
static void
ipaddr_add(const uip_ipaddr_t *addr)
{
  uint16_t a;
  int i, f;
  for(i = 0, f = 0; i < sizeof(uip_ipaddr_t); i += 2) {
    a = (addr->u8[i] << 8) + addr->u8[i + 1];
    if(a == 0 && f >= 0) {
      if(f++ == 0) {
        ADD("::");
      }
    } else {
      if(f > 0) {
        f = -1;
      } else if(i > 0) {
        ADD(":");
      }
      ADD("%x", a);
    }
  }
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(generate_routes(struct httpd_state *s))
{
  static uip_ds6_nbr_t *nbr;

  PSOCK_BEGIN(&s->sout);
  SEND_STRING(&s->sout, TOP);

  ADD("  Neighbors\n  <ul>\n");
  SEND(&s->sout);
  for(nbr = uip_ds6_nbr_head();
      nbr != NULL;
      nbr = uip_ds6_nbr_next(nbr)) {
    ADD("    <li>");
    ipaddr_add(&nbr->ipaddr);
    ADD("</li>\n");
    SEND(&s->sout);
  }
  ADD("  </ul>\n");
  SEND(&s->sout);
#if (UIP_MAX_ROUTES != 0)
  {
    static uip_ds6_route_t *r;
    ADD("  Routes\n  <ul>\n");
    SEND(&s->sout);
    for(r = uip_ds6_route_head(); r != NULL; r = uip_ds6_route_next(r)) {
      ADD("    <li>");
      ipaddr_add(&r->ipaddr);
      ADD("/%u (via ", r->length);
      ipaddr_add(uip_ds6_route_nexthop(r));
      ADD(") %lus", (unsigned long)r->state.lifetime);
      ADD("</li>\n");
      SEND(&s->sout);
    }
    ADD("  </ul>\n");
    SEND(&s->sout);
  }
#endif /* UIP_MAX_ROUTES != 0 */

#if (UIP_SR_LINK_NUM != 0)
  if(uip_sr_num_nodes() > 0) {
    static uip_sr_node_t *link;
    ADD("  Routing links\n  <ul>\n");
    SEND(&s->sout);
    for(link = uip_sr_node_head(); link != NULL; link = uip_sr_node_next(link)) {
      if(link->parent != NULL) {
        uip_ipaddr_t child_ipaddr;
        uip_ipaddr_t parent_ipaddr;
        NETSTACK_ROUTING.get_sr_node_ipaddr(&child_ipaddr, link);
        NETSTACK_ROUTING.get_sr_node_ipaddr(&parent_ipaddr, link->parent);

        ADD("    <li>");
        ipaddr_add(&child_ipaddr);

        ADD(" (parent: ");
        ipaddr_add(&parent_ipaddr);
        ADD(") %us", (unsigned int)link->lifetime);

        ADD("</li>\n");
        SEND(&s->sout);
      }
    }
    ADD("  </ul>");
    SEND(&s->sout);
  }
  uip_ipaddr_t root_ipaddr;
  NETSTACK_ROUTING.get_root_ipaddr(&root_ipaddr);
  ADD("    <li>");
  ADD("root=");
  ipaddr_add(&root_ipaddr);
  int numbers;
  numbers = uip_sr_num_nodes();
  printf("numbers of nodes is %d\r\n",numbers);

  ADD("<br />");
  ADD("<br />");
  ADD("<br />");
#endif /* UIP_SR_LINK_NUM != 0 */
  string[0] = string[0];
  treedata[0] = treedata[0];
  string2[0] = string2[0];
  //ADD(string,"%s");
  //ADD(treedata,"%s");
  //ADD(string2,"%s");

  SEND_STRING(&s->sout, BOTTOM);

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
PROCESS(webserver_nogui_process, "Web server");
PROCESS_THREAD(webserver_nogui_process, ev, data)
{
  PROCESS_BEGIN();

  httpd_init();

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
    httpd_appcall(data);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
httpd_simple_script_t
httpd_simple_get_script(const char *name)
{
  return generate_routes;
}
/*---------------------------------------------------------------------------*/
