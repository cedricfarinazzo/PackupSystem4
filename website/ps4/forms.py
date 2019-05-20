from django import forms
from .models import *

class BackupForm(forms.Form):
    backup_file = forms.FileField(widget=forms.ClearableFileInput(attrs={'multiple': True}))
    enc_type = forms.ChoiceField(
        required=True,
        choices=Backup.ENCRYPTION_TYPE,
    ) 
