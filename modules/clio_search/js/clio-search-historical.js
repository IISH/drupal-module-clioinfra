(function ($) {
  Drupal.behaviors.clioSearch = {
    attach: function (context, settings) {
      $('#edit-country', context)
        .once('clio-search', function () {

          // Add multiselect to select field, create separate search box above.
          $(this).multiSelect({
            selectableHeader: "<input id='geobox' type='text' class='search-input' autocomplete='off' placeholder=''>",
            afterDeselect: function(values){
              console.log("Deselect value: "+values);
              $('#ms-edit-country .ms-selectable .ms-list .ms-elem-selectable').hide();
            }
          });

          // Add typeahead to search box.
          $('#geobox').typeahead({
            dynamic: true,
            // delay: 300,
            minLength: 3,
            order: "asc",
            offset: true, // Country name should start with input string.
            accent: true, // Ignore accents.
            template: '<li class="ms-elem-selectable" id="{{id}}-selectable">' + 
              '<span>{{name}}</span></li>',
            display: "name",
            source: {
              url: {
                type: "GET",
                url: Drupal.settings.clioSearch.geocodeapi,
                data: {
                  name: "{{query}}",
                  after: function () { return $('#edit-year-min').val(); },
                  before: function () { return $('#edit-year-max').val(); }
                  //, myKey: "myValue" // Add API key here.
                }
              }
            },
            callback: {
              onClick: function (node, a, item, event) {
                // Add item and move to selected box.
                $('#edit-country')
                  .multiSelect('addOption', {value: item.id, text: item.name});
                $('#edit-country')
                  .multiSelect('select', item.id.toString());
                // Do not show selected item in input field.
                node[0].val("");
              },
            }
          });
        });
    }
  };
})(jQuery);

