import os
from django.core.exceptions import ObjectDoesNotExist
from django.dispatch import receiver
from django.utils.translation import ugettext_lazy as _
from django.db import models
from django.core.files.storage import FileSystemStorage
from django.contrib.auth.models import User
import uuid

def make_filepath(instance, filename):
    return "uploads/" + ("%s%s.%s" % (filename, uuid.uuid4(), filename.split('.')[-1]))


# Create your models here.


class StorageUser(models.Model):
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    usage = models.PositiveIntegerField(default=0)


class BackupFile(models.Model):
    ARCHIVE_PART = 'Archive part'
    PUBLIC_KEY = 'Public key'
    PRIVATE_KEY = 'Private key'
    LZ_DICO = 'Lz78 Dictionnary'
    BACKUP_FILE_TYPE = (
        (ARCHIVE_PART, 'Archive part'),
        (PUBLIC_KEY, 'Public key'),
        (PRIVATE_KEY, 'Private ley'),
        (LZ_DICO, 'Lz78 Dictionnary'),
    )
    file_type = models.CharField(
        max_length=42,
        choices=BACKUP_FILE_TYPE,
        default=ARCHIVE_PART,
    )
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    backup = models.ForeignKey('Backup', on_delete=models.CASCADE)
    backupfile = models.FileField(upload_to=make_filepath)
    filename = models.TextField(null=True, blank=True)
    
    @property
    def normal_size(self):
        size = float(self.backupfile.size)
        if size < 1000:
            return str(size)
        elif size < 1000000:
            size /= 1000
            return ('%.2f'%(size)) + 'Ko'
        elif size < 1000000000:
            size /= 1000000
            return ('%.2f'%(size)) + 'Mo'
        elif size < 1000000000000:
            size /= 1000000000
            return ('%.2f'%(size)) + 'Go'
        else:
            size /= 1000000000000
            return ('%.2f'%(size)) + 'To'

# Auto-delete files from filesystem when they are unneeded
@receiver(models.signals.post_delete, sender=BackupFile)
def auto_delete_file_on_delete(sender, instance, **kwargs):
    """
    Deletes file from filesystem
    when corresponding `BackupFile` object is deleted.
    """
    if instance.backupfile:
        if os.path.isfile(instance.backupfile.path):
            try:
                storage = StorageUser.objects.get(user=instance.user)
                storage.usage -= instance.backupfile.size
                storage.save()
            except ObjectDoesNotExist:
                pass            
            os.remove(instance.backupfile.path)


class ArchiveContent(models.Model):
    backupfile = models.ForeignKey(BackupFile, on_delete=models.CASCADE)
    SUCCESS = 'succes'
    ERROR = 'error'
    WAITING = 'waiting'
    STATUS_TYPE = (
        (SUCCESS, 'succes'),
        (ERROR, 'error'),
        (WAITING, 'waiting'),
    )
    status_type = models.CharField(
            max_length=20,
            choices=STATUS_TYPE,
            default=WAITING
        )
    content = models.TextField()


class Backup(models.Model):
    NONE = 'None'
    ROTN = 'Rotn'
    VIG = 'Vigenere'
    AES = 'AES'
    RSA = 'RSA'
    EL = 'Elgamal'
    ENCRYPTION_TYPE = (
        (NONE, 'None'),
        (ROTN, 'Rotn'),
        (VIG, 'Vigenere'),
        (AES, 'AES'),
        (RSA, 'RSA'),
        (EL, 'Elgamal'),
    )
    enc_type = models.CharField(
        max_length=11,
        choices=ENCRYPTION_TYPE,
        default=NONE,
    )
    HUFF = 'Huffman'
    LZ78 = 'Lz78'
    COMPRESSION_TYPE = (
        (NONE, 'None'),
        (HUFF, 'Huffman'),
        (LZ78, 'Lz78'),
    )
    comp_type = models.CharField(
        max_length=11,
        choices=COMPRESSION_TYPE,
        default=NONE,
    )
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    date_created = models.DateTimeField(auto_now_add=True)
    pass_backup = models.TextField(null=True, blank=True)

    @property
    def get_files(self):
        return BackupFile.objects.filter(backup=self).all()
    
    def get_archives(self):
        return BackupFile.objects.filter(backup=self, file_type=BackupFile.ARCHIVE_PART).all()

    def get_archive_content(self):
        archives = self.get_archives()
        if archives is None or archives == []:
            return []
        return ArchiveContent.objects.filter(backupfile=archives[0]).first()

    def get_lz_dico(self):
        return BackupFile.objects.filter(backup=self, file_type=BackupFile.LZ_DICO).first()

    def get_pub_key(self):
        return BackupFile.objects.filter(backup=self, file_type=BackupFile.PUBLIC_KEY).first()
    
    def get_priv_key(self):
        return BackupFile.objects.filter(backup=self, file_type=BackupFile.PRIVATE_KEY).first()

