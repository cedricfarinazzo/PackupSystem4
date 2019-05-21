from django import forms
from .models import *

class BackupForm(forms.Form):
    backup_file = forms.FileField(required=True, widget=forms.ClearableFileInput(attrs={'multiple': True}))
    enc_type = forms.ChoiceField(
        required=True,
        choices=Backup.ENCRYPTION_TYPE,
    )
    comp_type = forms.ChoiceField(
        required=True,
        choices=Backup.COMPRESSION_TYPE,
    )
    backup_pass = forms.CharField(required=False)
    backup_public_key_file = forms.FileField(required=False)
    backup_private_key_file = forms.FileField(required=False)
    backup_dico_file = forms.FileField(required=False)

