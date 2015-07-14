(function($) {

  Drupal.behaviors.carReferenceFieldAdminSelect = {
    attach: function(context) {

      $('.ajax-vocabulary-conf-vocabulary select').each(function(index) {
        appendVocabularyOptions($(this));
      });

      $('.ajax-vocabulary-conf-vocabulary select', context).bind('change', function(context) {
        appendVocabularyOptions($(this));
      });


      // Replace the options depending on chosen value in parent select.
      function appendVocabularyOptions(selectEl) {
        var vocab = selectEl.val(),
                parentWrapper = selectEl.closest('.fieldset-wrapper'),
                url = '/car_reference/vocabulary_fields/' + vocab;

        $.get(url, function(data) {
          var current_select = parentWrapper.find('.ajax-vocabulary-conf-ref-field-name select'),
                  selectedOption = current_select.find("option:selected").attr('value');

          // Clean-Up current options.
          current_select.empty();

          // Loop returned json options and append them as options.
          $.each(data, function(key, value) {
            if (selectedOption == key) {
              current_select.append($("<option></option>")
                      .attr('value', key)
                      .attr('selected', 'selected')
                      .text(value));
            }
            else {
              current_select.append($("<option></option>")
                      .attr('value', key)
                      .text(value));
            }

          });
        });
      };

    }
  };

})(jQuery);
