$(document).ready(function(){
    $('.sidenav').sidenav()
    $('select').formSelect();
    
    //Optional parameters | backup add
    $('#id_enc_type').on('change', function() {
        if (this.value == 'Rotn' || this.value == 'Vigenere' || this.value == 'AES') {
            $('#backup_key_container').fadeOut();
            $('#backup_pass_container').fadeIn();
        } else if (this.value == 'RSA' || this.value == 'Elgamal') {
            $('#backup_key_container').fadeIn();
            $('#backup_pass_container').fadeOut();
        } else {
            $('#backup_key_container').fadeOut();
            $('#backup_pass_container').fadeOut();
        }
    });
});
