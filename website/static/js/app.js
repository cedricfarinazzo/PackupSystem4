$(document).ready(function(){
    $('.sidenav').sidenav()
    $('select').formSelect();
    
    //Optional parameters | backup add
    $('#id_enc_type').on('change', function() {
        if (this.value == 'Rotn' || this.value == 'Vigenere' || this.value == 'AES') {
            $('#backup_key_container').fadeOut("fast", "swing");
            $('#backup_pass_container').fadeIn("slow", "swing");
        } else if (this.value == 'RSA' || this.value == 'Elgamal') {
            $('#backup_pass_container').fadeOut("fast", "swing");
            $('#backup_key_container').fadeIn("slow", "swing");
        } else {
            $('#backup_key_container').fadeOut("fast", "swing");
            $('#backup_pass_container').fadeOut("slow", "swing");
        }
    });

    //Archive content
    if ($('#backup_id_container').length > 0) {
        function load_archive_content() {
            var backup_id = $('#backup_id_container').text();
            $.ajax("/backup/view/content/"+backup_id+"/")
                .fail(function() {
                    console.log("failed to load archive content");
                    setTimeout(load_archive_content, 5000);
                })
                .done(function(data, status){
                    $('#backup_content_progress_container').fadeOut("slow", "swing");
                    $('#backup_content_text_container').html(data);
            });
        };
        setTimeout(load_archive_content, 2500);
    }
});
