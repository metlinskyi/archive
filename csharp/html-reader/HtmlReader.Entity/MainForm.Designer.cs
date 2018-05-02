namespace HtmlReaderEntity
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.cbxWebSite = new System.Windows.Forms.ComboBox();
            this.TimerProcess = new System.Windows.Forms.Timer(this.components);
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabLinks = new System.Windows.Forms.TabPage();
            this.tabImages = new System.Windows.Forms.TabPage();
            this.tabSource = new System.Windows.Forms.TabPage();
            this.tabControl1.SuspendLayout();
            this.SuspendLayout();
            // 
            // cbxWebSite
            // 
            this.cbxWebSite.FormattingEnabled = true;
            this.cbxWebSite.Location = new System.Drawing.Point(12, 14);
            this.cbxWebSite.Name = "cbxWebSite";
            this.cbxWebSite.Size = new System.Drawing.Size(396, 21);
            this.cbxWebSite.TabIndex = 0;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabLinks);
            this.tabControl1.Controls.Add(this.tabImages);
            this.tabControl1.Controls.Add(this.tabSource);
            this.tabControl1.Location = new System.Drawing.Point(12, 49);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(768, 497);
            this.tabControl1.TabIndex = 1;
            // 
            // tabLinks
            // 
            this.tabLinks.Location = new System.Drawing.Point(4, 22);
            this.tabLinks.Name = "tabLinks";
            this.tabLinks.Padding = new System.Windows.Forms.Padding(3);
            this.tabLinks.Size = new System.Drawing.Size(760, 471);
            this.tabLinks.TabIndex = 0;
            this.tabLinks.Text = "Links";
            this.tabLinks.UseVisualStyleBackColor = true;
            // 
            // tabImages
            // 
            this.tabImages.Location = new System.Drawing.Point(4, 22);
            this.tabImages.Name = "tabImages";
            this.tabImages.Padding = new System.Windows.Forms.Padding(3);
            this.tabImages.Size = new System.Drawing.Size(760, 471);
            this.tabImages.TabIndex = 1;
            this.tabImages.Text = "Images";
            this.tabImages.UseVisualStyleBackColor = true;
            // 
            // tabSource
            // 
            this.tabSource.Location = new System.Drawing.Point(4, 22);
            this.tabSource.Name = "tabSource";
            this.tabSource.Size = new System.Drawing.Size(760, 471);
            this.tabSource.TabIndex = 2;
            this.tabSource.Text = "Source";
            this.tabSource.UseVisualStyleBackColor = true;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(792, 566);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.cbxWebSite);
            this.Name = "MainForm";
            this.Text = "Html";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.tabControl1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox cbxWebSite;
        private System.Windows.Forms.Timer TimerProcess;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabLinks;
        private System.Windows.Forms.TabPage tabImages;
        private System.Windows.Forms.TabPage tabSource;
    }
}

