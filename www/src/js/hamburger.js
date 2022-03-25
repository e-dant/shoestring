!function($) {
    "use strict";
    $(document).on("click", ".leftbar-hamburger", function(e) {
        e.preventDefault();
        $(".leftbar").toggleClass("hidden")
    })
}(window.jQuery);
