(function ($) {
  Drupal.behaviors.clioAnalyzeGraphs = {
    attach: function (context, settings) {
      $('div.' + settings.clioAnalyzeGraphs.div, context)
        .once('clio-analyze-graphs', function () {
          showgraph();
        });
    }
  };
})(jQuery);

function showgraph() {

  var div    = Drupal.settings.clioAnalyzeGraphs.div;
  var handle = Drupal.settings.clioAnalyzeGraphs.handle;
  var graphapi = Drupal.settings.clioAnalyzeGraphs.graphapi;

  var margin = {top: 20, right: 20, bottom: 30, left: 40},
    width = 800 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

  var x = d3.scale.ordinal()
    .rangeRoundBands([0, width], .1);

  var y = d3.scale.linear()
    .range([height, 0]);

  var xAxis = d3.svg.axis()
    .scale(x)
    .orient("bottom");

  var yAxis = d3.svg.axis()
    .scale(y)
    .orient("left")

  var svg = d3.select(div).append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  d3.tsv(graphapi + "?dataset=" + handle, type, function(error, data) {
    x.domain(data.map(function(d) { return d.digit; }));
    y.domain([0, d3.max(data, function(d) { return d.frequency; })]);

    svg.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + height + ")")
      .call(xAxis)
      .append("text")
      .attr("y", 25)
      .attr("x", width - 40)
      .text("First Digit");

    svg.append("g")
      .attr("class", "y axis")
      .call(yAxis)
      .append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 1)
      .attr("dy", ".71em")
      .style("text-anchor", "end")
      .text("Frequency");

    svg.selectAll(".bar")
      .data(data)
      .enter().append("rect")
      .attr("class", "bar")
      .attr("x", function(d) { return x(d.digit); })
      .attr("width", x.rangeBand())
      .attr("y", function(d) { return y(d.frequency); })
      .attr("height", function(d) { return height - y(d.frequency); });

  });

  function type(d) {
    d.frequency = +d.frequency;
    return d;
  }

}
