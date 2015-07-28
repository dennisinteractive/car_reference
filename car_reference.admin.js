(function ($) {

    Drupal.behaviors.carReferenceFieldAdminSelect = {
        attach: function (context) {

            // Replace the options depending on chosen value in parent select.
            var detectVocabularyOptions = {
                ajaxBaseUrl: '/car_reference/vocabulary_fields/',
                noWayToEdit: Drupal.settings.car_reference.noWayToEdit.length > 0,

                detectOptions: function () {

                    this.currentSelect = this.parentWrapper.find('.ajax-vocabulary-conf-ref-field-name select');
                    this.selectedOption = this.currentSelect.find("option:selected").attr('value');

                    // Clean-Up current options.
                    this.currentSelect.empty();

                    var currSelectEl = this.currentSelect;
                    var currSelectedOption = this.selectedOption;

                    // This means that this field configuration can't be edited anymore due to populated records in the
                    // current table. This means that all the configuration needs to be done programmatically.
                    if (this.noWayToEdit) {
                        var customOptions = {
                            'none': Drupal.t('Error: The field is locked and/or not all field settings are set.'),
                        };

                        // Append the options.
                        this.appendOptions(customOptions, currSelectEl, currSelectedOption);
                        return;
                    }

                    var tempThis = this;
                    $.get(this.ajaxUrl, function (data) {
                        // Append the options.
                        tempThis.appendOptions(data, currSelectEl, currSelectedOption);
                    });
                },

                // Loop returned JSON options and append those as options.
                appendOptions: function (data, currSelectEl, currSelectedOption) {
                    var tempThis = this;
                    $.each(data, function (key, value) {

                        if (currSelectedOption == key) {
                            currSelectEl.append($("<option></option>")
                                .attr('value', key)
                                .attr('selected', 'selected')
                                .text(value));
                        }
                        else {
                            currSelectEl.append($("<option></option>")
                                .attr('value', key)
                                .text(value));
                        }

                    });
                },

                // detectVocabularyOptions initialization function.
                // @selectEl - select html element object.
                init: function (selectEl) {
                    // Initiate needed variables.
                    this.vocab = selectEl.val();
                    this.parentWrapper = selectEl.closest('.fieldset-wrapper');
                    this.ajaxUrl = this.ajaxBaseUrl + this.vocab;
                    // Process the select element if all the data is available.
                    if (this.vocab.length && this.parentWrapper.length) {
                        this.detectOptions();
                    }
                }
            }

            // Initial call.
            $('.ajax-vocabulary-conf-vocabulary select').each(function (index) {
                // Skip the make, as it doesn't make sense.
                if (index !== 0) {
                    detectVocabularyOptions.init($(this));
                }
            });

            // On Change call.
            $('.ajax-vocabulary-conf-vocabulary select', context).bind('change', function (context) {
                detectVocabularyOptions.init($(this));
            });
        }
    };

})(jQuery);
