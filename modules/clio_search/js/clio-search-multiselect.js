(function ($) {
  Drupal.behaviors.clioSearch = {
    attach: function (context, settings) {
      $('#edit-indicator', context)
        .once('clio-search-multiselect', function () {
        
          var searchbox = "selectableHeader: \"<input type='text' class='search-input' autocomplete='off' placeholder=''>\",\
          afterInit: function(ms){\
            var that = this,\
                $selectableSearch = that.$selectableUl.prev(),\
                selectableSearchString = '#'+that.$container.attr('id')+' .ms-elem-selectable:not(.ms-selected)';\
            that.qs1 = $selectableSearch.quicksearch(selectableSearchString)\
            .on('keydown', function(e){\
              if (e.which === 40){\
                that.$selectableUl.focus();\
                return false;\
              }\
            });\
          },\
          afterSelect: function(){\
            this.qs1.cache();\
          },\
          afterDeselect: function(){\
            this.qs1.cache();\
          }\
          ";        
        
          $(this).multiSelect(searchbox);
        });
    }
  };
})(jQuery);
