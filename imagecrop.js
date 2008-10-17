/* $Id$ */

/*
 * Toolbox @copyright from imagefield_crop module with some minor modifications.
 * To be used with Jquery UI.
 */

$(document).ready(function(){
	if ($('#resizeMe').resizable) {
	  $('#resizeMe').resizable({
		containment: $('#image-crop-container'),
		//proxy: 'proxy',
		//ghost: true,
		//animate:true,
		handles: 'all',
		knobHandles: true,
		//transparent: true,
		aspectRatio: false,
		autohide: true,

		resize: function(e, ui) {
  		  this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
		  $("#edit-image-crop-width").val($('#resizeMe').width());
		  $("#edit-image-crop-height").val($('#resizeMe').height());
		  $("#edit-image-crop-x").val(ui.position.left);
		  $("#edit-image-crop-y").val(ui.position.top);
  	    },
		stop: function(e, ui) {
  		  this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
  	    }
	  })
	}

	$('#resizeMe').draggable({
		cursor: 'move',
		containment: $('#image-crop-container'),
		drag: function(e, ui) {
		  this.style.backgroundPosition = '-' + (ui.position.left) + 'px -' + (ui.position.top) + 'px';
		  $("#edit-image-crop-x").val(ui.position.left);
		  $("#edit-image-crop-y").val(ui.position.top);
		}
	});
	
	$('#image-crop-container').css({ opacity: 0.5 });
    $("#resizeMe").left($('#edit-image-crop-x').val());
    $("#resizeMe").top($('#edit-image-crop-y').val());
    var leftpos = $('#edit-image-crop-x').val();
    var toppos = $('#edit-image-crop-y').val();
    $("#resizeMe").css({backgroundPosition: '-'+ leftpos + 'px -'+ toppos +'px'});
});

$(window).load(function(){
  $("#resizeMe").width($('#edit-image-crop-width').val());
  $("#resizeMe").height($('#edit-image-crop-height').val());
});
