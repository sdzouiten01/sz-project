/**
*  First letter uppercase
*/
String.prototype.ucfirst = function()
{
    return this.charAt(0).toUpperCase() + this.substr(1);
}

/**
 * Bool to Checkbox
 */
 String.prototype.toChecked = function() {
  return (this == '1' || this == 'on' || this == 'checked') ? "checked" : "";
 }


/**
*  Rounds number
*/
function round_up(val, precision) {
  power = Math.pow (10, precision);
  poweredVal = Math.ceil (val * power);
  result = poweredVal / power;
  return result;
}

/**
*  Format file size
*/
function formatFileSize(filesize) {
  if (typeof filesize !== 'number') {
    return '';
  }
  var sizes = new Array('bytes', 'kb', 'mb', 'gb', 'tb', 'pb', 'eb', 'zb', 'yb');
  if (filesize == 0) return('n/a');
  return round_up(filesize / Math.pow(1024, i = Math.floor(Math.log(filesize) / Math.log(1024))), 2) + ' ' + sizes[i];
}

/**
*  Fadein delay plugin
*/
jQuery.fn.fadeInDelay = function(options){

  var settings = {
    'delaytime' : 195,
    'speed'     : 500,
    'animate'   : { opacity : 1 },
    'initial'   : { opacity : 0 },
    'completed' : function(target) { }    
  };  
  var delay = 0;
  return this.each(function(){
  // If options exist, lets merge them
  // with our default settings
  if ( options ) { 
    jQuery.extend( settings, options );
  }
  //Hide all elements
  jQuery(this).css(settings.initial);
  //Does the fading    
  jQuery(this).delay(delay).animate(settings.animate,settings.speed,settings.completed);
    delay += settings.delaytime;
  });    
};

/**
*  Digits only plugin
*/
jQuery.fn.digitsOnly = function(){
  
  $(this).live('keydown', function(event) {
    
    if (event.keyCode == 46 || event.keyCode == 8 || event.keyCode == 9 || event.keyCode == 27 || event.keyCode == 13 || (event.keyCode == 65 && event.ctrlKey === true) || (event.keyCode >= 35 && event.keyCode <= 39)) {
      return;
    }
    else {
    
      if (event.shiftKey || (event.keyCode < 48 || event.keyCode > 57) && (event.keyCode < 96 || event.keyCode > 105)) {
        event.preventDefault();
      }
    }
    
  });
  
};

/**
*  jQuery intitializer
*/
$(function(){
  
  $('.digitsonly').digitsOnly();

  // Custom selects
  $(".select select").live('changeLabel', function() {
    var value = $('option:selected', this).text();
    $(this).closest('.select').find('.label').text(value);
  });
  $(".select select").live('change', function() {
    $(this).trigger('changeLabel');
  });
  
  // search interaction on
  $('.search').focus(function(e) {
    
    if(!$(this).data('width')) {
      $(this).data('width', $(this).width());
    }
    
    width = $(this).width();
    $(this).animate({'width': (width + (width * 0.3))}, 'fast');
    
  });
  
  // search interaction off
  $('.search').blur(function(e) {
    
    width = $(this).width();
    $(this).animate({'width': $(this).data('width')}, 'fast');
    
  });
  
  // share links
  $('.share').click(function() {
    var width  = $(this).attr('data-width'),
        height = $(this).attr('data-height'),
        left   = ($(window).width()  - width)  / 2,
        top    = ($(window).height() - height) / 2,
        url    = this.href,
        opts   = 'status=1' +
                 ',width='  + width  +
                 ',height=' + height +
                 ',top='    + top    +
                 ',left='   + left;
    window.open(url, 'twitter', opts);    
    return false;
  });
  
  // email share
  $('.emailshare').click(function(e) {
  
    var _this = $(this).find('form').toggle();
    
    $(this).find('form').bind('click.emailshare', function(e) {
      e.stopPropagation();
    });
    
    e.stopImmediatePropagation();
    
    $(document).bind('click.emailshare', function(e) {
    
      _this.hide().find('span.message').remove();
      _this.find('.field input,textarea').val('');
      
    });
  
  });
  
  // sharebar
  $('.sharebar').click(function(e) {
    
    $(this).find('.buttons').toggle();
    
  });
  
  // ajax form
  $('.ajaxform').live('submit', function(e) {
    
    e.preventDefault();
    
    var form = $(e.target);
    
    form.find('span.message').remove();
    
    //Submits the ajax contact form and displays the result
    $.post(form.attr('action'), form.serialize(), function(msg, b, c) {
      form.append('<span class="message success">Message Sent</span>');
    }).error(function(response, type, error) {
      form.append('<span class="message error">'+response.responseText+'</span>');
    });
    
  });
  
  // image expand interaction
  $('.upload.single .thumb img').click(function(e) {
  
    $('.thumb').css('width', $(window).width());
    
    $('.thumb').css('height', $(window).height());
  
    if($('body').hasClass('expanded')) {
      
      $('body').removeClass('expanded');
      
      $('.thumb').css('width', '');
      
      $('.thumb').css('height', '');
      
      $(window).unbind('resize');
      
    }
    
    else {
      
      $('body').addClass('expanded');
      
      $(window).bind('resize', function() {
        
        $('.thumb').css('width', $(window).width());
        
        $('.thumb').css('height', $(window).height());
        
      });
      
    }
    
  });

});