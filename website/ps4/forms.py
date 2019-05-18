from django import forms
from .models import *

class BackupForm(forms.Form):
    backup_file = forms.FileField()
    enc_type = forms.ChoiceField(
        required=True,
        choices=Backup.ENCRYPTION_TYPE,
    ) 
