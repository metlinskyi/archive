using Matrix.Handlers;
using Matrix.Services;
using Matrix.Web.Models;
using Microsoft.VisualBasic.Devices;
using System;
using System.Web;
using System.Web.Mvc;

namespace Matrix.Web.Controllers
{
    public class MatrixController : Controller
    {
        private readonly IMatrixStorage _matrixStorage;

        private readonly IMatrixSerializer _matrixSerializer;

        public MatrixController(IMatrixStorage matrixStorage, IMatrixSerializer matrixSerializer)
        {
            _matrixStorage = matrixStorage;
            _matrixSerializer = matrixSerializer;
        }

        /// <summary>
        /// Грид показывает матрицу
        /// </summary>
        [HttpGet]
        public ActionResult Index(Guid? id)
        {
            if (!id.HasValue)
                return RedirectToAction("Index", "Default");

            var matrix = _matrixStorage.Get<int>(id.Value);

            return View((MatrixModel)matrix);
        }

        /// <summary>
        /// Сгенерировать рандомную матрицу;
        /// </summary>
        [HttpGet]
        public ActionResult Create()
        {
            var matrix = Matrix.Create<int>(GetMatrixSize).Handling(new MatrixRandomizer<int>(r => r.Next()));

            Guid id = _matrixStorage.Put<int>(matrix);

            return RedirectToAction("Index", new { id });
        }

        /// <summary>
        /// Повернуть матрицу на 90 градусов;
        /// </summary>
        [HttpGet]
        public ActionResult Rotate(Guid id)
        {
            var matrix = _matrixStorage.Get<int>(id).Handling(new MatrixRotation<int>());

            return RedirectToAction("Index", new { id });
        }

        /// <summary>
        /// Загрузить матрицу из файла .csv;
        /// </summary>
        [HttpPost]
        public ActionResult Import(HttpPostedFileBase file)
        {
            var matrix = _matrixSerializer.Deserialize<int>(file.InputStream, file.ContentType);

            Guid id = _matrixStorage.Put<int>(matrix);

            return RedirectToAction("Index", new { id });
        }

        /// <summary>
        /// Экспортировать матрицу в файл .csv;
        /// </summary>
        [HttpPost]
        public ActionResult Export(Guid id, string contentType = "text/csv")
        {
            var matrix = _matrixStorage.Get<int>(id);

            return File(_matrixSerializer.Serialize<int>(matrix, contentType), contentType);
        }

        private static int GetMatrixSize
        {
            get
            {
                int max;

                unchecked
                {
                    max = (int)((GC.GetTotalMemory(true) / sizeof(int)) >> 8);
                }

                max = max == -1 ? int.MaxValue : max;

                return max;
            }
        }
    }
}