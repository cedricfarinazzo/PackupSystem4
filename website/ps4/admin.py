from django.contrib import admin
from .models import *

# Register your models here.

class BackupFileAdmin(admin.ModelAdmin):
    model = BackupFile
    list_display = ('id', 'filename', 'backupfile', 'backup', 'file_type')
admin.site.register(BackupFile, BackupFileAdmin)

class BackupAdmin(admin.ModelAdmin):
    model = Backup
    list_display = ('id', 'user', 'enc_type', 'comp_type', 'date_created')
admin.site.register(Backup, BackupAdmin)

class ArchiveContentAdmin(admin.ModelAdmin):
    model = ArchiveContent
    list_display = ('id', 'backupfile', 'status_type', 'content')
admin.site.register(ArchiveContent, ArchiveContentAdmin)

class StorageUserAdmin(admin.ModelAdmin):
    model = StorageUser
    list_display = ('id', 'user', 'usage')
admin.site.register(StorageUser, StorageUserAdmin)
