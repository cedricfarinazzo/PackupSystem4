from django.contrib import admin
from .models import *

# Register your models here.

class BackupAdmin(admin.ModelAdmin):
    model = Backup
    list_display = ('user', 'backup', 'filename', 'enc_type', 'date_created')
admin.site.register(Backup, BackupAdmin)

class StorageUserAdmin(admin.ModelAdmin):
    model = StorageUser
    list_display = ('user', 'usage')
admin.site.register(StorageUser, StorageUserAdmin)
